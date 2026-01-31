/*****************************************************************************************
 *  EmergiNet – Wio Terminal Meshtastic Receiver
 *
 *  Description:
 *  This firmware runs on a Seeed Studio Wio Terminal connected via UART (Serial1)
 *  to a Meshtastic node. It listens for incoming text messages broadcast over the
 *  mesh network and selectively displays emergency-related information.
 *
 *  Core Behavior:
 *  - Messages are received line-by-line over UART from the Meshtastic node.
 *  - A display “message block” begins when a line contains the keyword:
 *        "EmergiNet"
 *  - Once a block starts, all immediately consecutive lines are displayed on the LCD.
 *  - The block ends when a blank line is received or after a short inactivity timeout.
 *  - Messages that do NOT belong to an EmergiNet block are ignored and never displayed.
 *
 *  Audible Alerts:
 *  - If any incoming line contains the command string:
 *        "xSiren!E1"
 *    an emergency siren is activated for 15 seconds.
 *  - The siren can be retriggered multiple times.
 *  - Normal message beeps occur only for valid EmergiNet display lines.
 *  - No buzzer activity occurs for ignored messages.
 *
 *  User Interaction:
 *  - The 5-way joystick (UP / DOWN / PRESS) is used to scroll messages.
 *  - Any intentional joystick press sends a JSON "RECEIVED" acknowledgment.
 *
 *  IMPORTANT: False-Trigger Prevention (RECEIVED)
 *  - Some systems can see brief LOW glitches on joystick pins (noise/ground bounce),
 *    sometimes coinciding with radio/UART activity.
 *  - To ensure RECEIVED is sent only when physically pressed:
 *      1) We confirm the pin remains LOW for PRESS_CONFIRM_MS (debounce/filter).
 *      2) We require a full release (back to HIGH) before re-arming the send.
 *      3) ACK_LOCKOUT_MS still applies as a minimum spacing between sends.
 *
 *  Startup Safety:
 *  - Previous joystick states are initialized in setup() to avoid a false "press"
 *    immediately at boot.
 *
 *  Display:
 *  - Green text on black background.
 *  - Messages are stored in a circular buffer and scroll automatically.
 *
 *  Hardware:
 *  - Platform: Seeed Studio Wio Terminal
 *  - Display: TFT (TFT_eSPI)
 *  - Buzzer: Onboard Wio buzzer
 *  - Radio link: External Meshtastic node via UART (Serial1, 115200 baud)
 *
 *  Project:
 *  EmergiNet – Community Emergency Notification System
 *
 ****************************************************************************************
 *  Updates in this version:
 *  - External siren on D7 using simple HIGH/LOW toggling (NO tone)
 *      Total: 10 seconds
 *      Pattern: 1.0 sec ON, 0.5 sec OFF
 *      Retriggerable: each "xSiren!E1" restarts the 10-second pattern
 *  - Removed old tone/noTone siren code
 *  - "RECEIVED" ACK is sent as plain text (works with Meshtastic serial.mode=TEXTMSG)
 *
 *****************************************************************************************/

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft;

// ---- Pins ----
#define PIN_BUZZER    WIO_BUZZER
#define PIN_5S_UP     WIO_5S_UP
#define PIN_5S_DOWN   WIO_5S_DOWN
#define PIN_5S_LEFT   WIO_5S_LEFT
#define PIN_5S_RIGHT  WIO_5S_RIGHT
#define PIN_5S_PRESS  WIO_5S_PRESS
#define PIN_BTN_A     WIO_KEY_A
#define PIN_BTN_B     WIO_KEY_B
#define PIN_BTN_C     WIO_KEY_C

#define EX_BUZ_PIN    D7   // External siren control pin on Wio Terminal

// ---- Serial from Heltec Meshtastic ----
#define MSG_SERIAL    Serial1
#define BAUDRATE      115200   // Meshtastic default over UART

// ---- Target Meshtastic node (optional; not used in TEXTMSG mode) ----
const char* TARGET_NODE = "!9ea2084c";

// ---- Message buffer ----
const int MAX_LINES    = 30;
const int MAX_LINE_LEN = 120;

char messages[MAX_LINES][MAX_LINE_LEN];
int totalMessages = 0;          // number of stored messages (0..MAX_LINES)
int writeIndex    = 0;          // circular write index
int scrollIndex   = 0;          // index of first visible message

// ---- Display layout ----
const int TEXT_SIZE     = 2;
const int LINE_HEIGHT   = 22;
const int VISIBLE_LINES = 10;

// ---- Buzzer / normal alert beep for new displayed lines ----
bool alarmActive         = false;
unsigned long alarmEndAt = 0;
unsigned long lastToggle = 0;
const unsigned long ALARM_MS  = 5000;   // beep duration for each new displayed line
const unsigned long TOGGLE_MS = 250;    // beep on/off period

// ---- 5-way edge detection + lockout ----
int prevUp    = HIGH;
int prevDown  = HIGH;
int prevPress = HIGH;

unsigned long lastAckAt = 0;
const unsigned long ACK_LOCKOUT_MS = 400;

// ---- Temp buffer to assemble UART line ----
char inbuf[MAX_LINE_LEN];
int  inpos = 0;

// ===== Multi-line block state for EmergiNet =====
bool inEmergiNetBlock = false;
unsigned long lastBlockLineAt = 0;
const unsigned long BLOCK_TIMEOUT_MS = 1500;  // gap that ends a block

// ===== Stronger physical-press gating =====
bool ackArmed = true;                       // only allow send when armed
const unsigned long PRESS_CONFIRM_MS = 35;  // must remain LOW for this long to count

// ===== External siren on D7 (simple HIGH/LOW toggling) =====
bool extSirenActive = false;
unsigned long extSirenStartAt = 0;
unsigned long extSirenNextToggleAt = 0;
bool extSirenIsHigh = false;

const unsigned long EXT_SIREN_TOTAL_MS = 10000UL; // 10 seconds total
const unsigned long EXT_SIREN_ON_MS    = 1000UL;  // 1.0 sec ON
const unsigned long EXT_SIREN_OFF_MS   = 500UL;   // 0.5 sec OFF

// ------------------------------------------------------------------
// Send "RECEIVED" message back over Serial1 (TEXTMSG mode expects plain text)
// ------------------------------------------------------------------
void sendReceivedMessage() {
  MSG_SERIAL.print("RECEIVED\n");
}

void helpMessages() {
  MSG_SERIAL.print("HELP!\n");
}

// Confirm a physical press (filters noise spikes)
// NOTE: This uses a short delay; usually OK, but keep it short.
bool confirmPressLow(int pin) {
  if (digitalRead(pin) != LOW) return false;
  delay(PRESS_CONFIRM_MS);
  return (digitalRead(pin) == LOW);
}

// ===== External siren controls =====
void startExternalSiren10s() {
  extSirenActive = true;
  extSirenStartAt = millis();

  extSirenIsHigh = true;
  digitalWrite(EX_BUZ_PIN, HIGH);

  extSirenNextToggleAt = extSirenStartAt + EXT_SIREN_ON_MS;
}

void stopExternalSiren() {
  extSirenActive = false;
  extSirenIsHigh = false;
  digitalWrite(EX_BUZ_PIN, LOW);
}

void updateExternalSiren() {
  if (!extSirenActive) return;

  unsigned long now = millis();

  // Stop after total duration
  if (now - extSirenStartAt >= EXT_SIREN_TOTAL_MS) {
    stopExternalSiren();
    return;
  }

  // Toggle on schedule
  if ((long)(now - extSirenNextToggleAt) >= 0) {
    extSirenIsHigh = !extSirenIsHigh;
    digitalWrite(EX_BUZ_PIN, extSirenIsHigh ? HIGH : LOW);

    extSirenNextToggleAt = now + (extSirenIsHigh ? EXT_SIREN_ON_MS : EXT_SIREN_OFF_MS);
  }
}

// ------------------------------------------------------------------
// Clear all stored messages and the screen
// ------------------------------------------------------------------
void clearMessages() {
  for (int i = 0; i < MAX_LINES; i++) {
    messages[i][0] = '\0';
  }
  totalMessages = 0;
  writeIndex    = 0;
  scrollIndex   = 0;

  tft.fillScreen(TFT_BLACK);
}

// ------------------------------------------------------------------
// Redraw screen with current messages and scrollIndex
// NOTE: This is the original rendering approach (linear indexing).
// If you later want perfect ordering after wrap-around, we can swap in
// the corrected ring-buffer rendering.
// ------------------------------------------------------------------
void renderScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(TEXT_SIZE);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  if (totalMessages == 0) return;

  int visible = totalMessages;
  if (visible > VISIBLE_LINES) visible = VISIBLE_LINES;

  int maxStart = (totalMessages > visible) ? (totalMessages - visible) : 0;
  if (scrollIndex < 0)        scrollIndex = 0;
  if (scrollIndex > maxStart) scrollIndex = maxStart;

  int y = 0;
  for (int i = 0; i < visible; i++) {
    int idx = scrollIndex + i;
    if (idx >= totalMessages) break;
    tft.setCursor(0, y);
    tft.println(messages[idx]);
    y += LINE_HEIGHT;
  }
}

// ------------------------------------------------------------------
// Store a new message
// - If line contains "xSiren!E1", trigger external siren (always).
// - Only display EmergiNet multi-line blocks.
// ------------------------------------------------------------------
void storeMessage(const char *msg) {
  if (msg == NULL) return;

  // External siren trigger (does NOT require EmergiNet)
  if (strstr(msg, "xSiren!E1") != NULL) {
    startExternalSiren10s();   // retriggerable
  }

  // Blank line ends EmergiNet block, not stored
  if (msg[0] == '\0') {
    inEmergiNetBlock = false;
    return;
  }

  bool hasEmergiNet = (strstr(msg, "EmergiNet") != NULL);

  if (!inEmergiNetBlock) {
    if (!hasEmergiNet) {
      // Ignore non-EmergiNet messages
      alarmActive = false;
      analogWrite(PIN_BUZZER, 0);
      return;
    }
    inEmergiNetBlock = true;
  }

  lastBlockLineAt = millis();

  // Save line
  strncpy(messages[writeIndex], msg, MAX_LINE_LEN - 1);
  messages[writeIndex][MAX_LINE_LEN - 1] = '\0';

  writeIndex = (writeIndex + 1) % MAX_LINES;
  if (totalMessages < MAX_LINES) {
    totalMessages++;
  }

  int visible  = (totalMessages > VISIBLE_LINES) ? VISIBLE_LINES : totalMessages;
  int maxStart = (totalMessages > visible) ? (totalMessages - visible) : 0;
  scrollIndex  = maxStart;

  // Start normal beep for new displayed line (independent from external siren)
  alarmActive  = true;
  alarmEndAt   = millis() + ALARM_MS;
  lastToggle   = 0;

  renderScreen();
}

// ------------------------------------------------------------------
// Build lines from UART. Each '\n'-terminated line = message.
// ------------------------------------------------------------------
void handleUart() {
  while (MSG_SERIAL.available() > 0) {
    char c = MSG_SERIAL.read();

    if (c == '\r') continue;

    if (c == '\n') {
      if (inpos > 0) {
        inbuf[inpos] = '\0';
        storeMessage(inbuf);
        inpos = 0;
      } else {
        // Blank line
        storeMessage("");
      }
    } else {
      if (inpos < (MAX_LINE_LEN - 1)) {
        inbuf[inpos++] = c;
      }
    }
  }
}

// ------------------------------------------------------------------
// Scrolling helpers
// ------------------------------------------------------------------
void scrollUp() {
  if (totalMessages == 0) return;
  if (scrollIndex > 0) {
    scrollIndex--;
    renderScreen();
  }
}

void scrollDown() {
  if (totalMessages == 0) return;

  int visible  = (totalMessages > VISIBLE_LINES) ? VISIBLE_LINES : totalMessages;
  int maxStart = (totalMessages > visible) ? (totalMessages - visible) : 0;

  if (scrollIndex < maxStart) {
    scrollIndex++;
    renderScreen();
  }
}

// ------------------------------------------------------------------
void setup() {
  tft.begin();
  tft.setRotation(3);

  pinMode(PIN_BUZZER, OUTPUT);
  analogWrite(PIN_BUZZER, 0);

  pinMode(PIN_5S_UP,     INPUT_PULLUP);
  pinMode(PIN_5S_DOWN,   INPUT_PULLUP);
  pinMode(PIN_5S_LEFT,   INPUT_PULLUP);
  pinMode(PIN_5S_RIGHT,  INPUT_PULLUP);
  pinMode(PIN_5S_PRESS,  INPUT_PULLUP);
  pinMode(PIN_BTN_A,     INPUT_PULLUP);
  pinMode(PIN_BTN_B,     INPUT_PULLUP);
  pinMode(PIN_BTN_C,     INPUT_PULLUP);

  pinMode(EX_BUZ_PIN, OUTPUT);
  digitalWrite(EX_BUZ_PIN, LOW);   // siren off at boot

  // Initialize previous joystick states to prevent startup false press
  prevUp    = digitalRead(PIN_5S_UP);
  prevDown  = digitalRead(PIN_5S_DOWN);
  prevPress = digitalRead(PIN_5S_PRESS);
  lastAckAt = millis();

  MSG_SERIAL.begin(BAUDRATE);

  clearMessages();
}

// ------------------------------------------------------------------
void loop() {
  // 1) Handle incoming messages
  handleUart();

  // End EmergiNet block if no new line arrives for a while
  if (inEmergiNetBlock) {
    unsigned long now = millis();
    if (now - lastBlockLineAt > BLOCK_TIMEOUT_MS) {
      inEmergiNetBlock = false;
    }
  }

  // 2) External siren update (non-blocking)
  updateExternalSiren();

  // 3) Scroll with 5-way switch + send RECEIVED (ONLY on confirmed physical press)
  int curUp    = digitalRead(PIN_5S_UP);
  int curDown  = digitalRead(PIN_5S_DOWN);
  int curPress = digitalRead(PIN_5S_PRESS);

  // Detect a NEW press (HIGH -> LOW) on any of the three
  bool newEvent =
    (prevUp    == HIGH && curUp    == LOW) ||
    (prevDown  == HIGH && curDown  == LOW) ||
    (prevPress == HIGH && curPress == LOW);

  // Physical-press confirmation + re-arm requirement
  if (newEvent && ackArmed) {
    bool realPress =
      (prevUp    == HIGH && curUp    == LOW && confirmPressLow(PIN_5S_UP)) ||
      (prevDown  == HIGH && curDown  == LOW && confirmPressLow(PIN_5S_DOWN)) ||
      (prevPress == HIGH && curPress == LOW && confirmPressLow(PIN_5S_PRESS));

    if (realPress) {
      unsigned long now = millis();
      if (now - lastAckAt >= ACK_LOCKOUT_MS) {
        sendReceivedMessage();
        lastAckAt = now;
        ackArmed = false; // disarm until joystick is released
      }
    }
  }

  // Re-arm only after all relevant inputs return HIGH (released)
  if (!ackArmed && curUp == HIGH && curDown == HIGH && curPress == HIGH) {
    ackArmed = true;
  }

  // Update previous states
  prevUp    = curUp;
  prevDown  = curDown;
  prevPress = curPress;

  // Keep scrolling behavior (only when held low is fine)
  if (curUp == LOW)   scrollUp();
  if (curDown == LOW) scrollDown();

  // 4) Clear screen and message history when A + B are pressed together
  if (digitalRead(PIN_BTN_A) == LOW && digitalRead(PIN_BTN_B) == LOW) {
    delay(100); // debounce
    if (digitalRead(PIN_BTN_A) == LOW && digitalRead(PIN_BTN_B) == LOW) {
      clearMessages();
      delay(300);
    }
  }

  if (digitalRead(PIN_BTN_C) == LOW && digitalRead(PIN_BTN_B) == LOW) {
    delay(100); // debounce
    if (digitalRead(PIN_BTN_C) == LOW && digitalRead(PIN_BTN_B) == LOW) {
      helpMessages();
      delay(300);
    }
  }

  // 5) Normal buzzer beep for new EmergiNet messages (independent of external siren)
  if (alarmActive) {
    unsigned long now = millis();
    if (now < alarmEndAt) {
      if (now - lastToggle >= TOGGLE_MS) {
        static bool on = false;
        on = !on;
        analogWrite(PIN_BUZZER, on ? 200 : 0);
        lastToggle = now;
      }
    } else {
      analogWrite(PIN_BUZZER, 0);
      alarmActive = false;
    }
  }

  delay(5);
}
