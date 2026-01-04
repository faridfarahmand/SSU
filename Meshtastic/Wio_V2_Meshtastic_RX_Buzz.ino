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
 *  - Any intentional joystick press sends a JSON "RECEIVED" acknowledgment
 *    back to the originating Meshtastic node.
 *  - Startup logic prevents accidental transmission until the user physically
 *    presses the joystick.
 *  - Buttons A + B together clear the screen and message history.
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
 *  Notes:
 *  - This code intentionally avoids JSON parsing libraries to remain lightweight.
 *  - All behavior is deterministic and event-driven.
 *
 *  Project:
 *  EmergiNet – Community Emergency Notification System
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

// ---- Serial from Heltec Meshtastic ----
#define MSG_SERIAL    Serial1
#define BAUDRATE      115200   // Meshtastic default over UART


// ---- Target Meshtastic node (replace with full ID that ends with 084c) ----
const char* TARGET_NODE = "!9ea2084c";   // TODO: put your actual full node ID here

// ---- Message buffer ----
const int MAX_LINES    = 30;    // store last 30 messages
const int MAX_LINE_LEN = 120;   // max characters per message

char messages[MAX_LINES][MAX_LINE_LEN];
int totalMessages = 0;          // number of stored messages (0..30)
int writeIndex    = 0;          // circular write index
int scrollIndex   = 0;          // index of first visible message

// ---- Display layout (match your green example) ----
const int TEXT_SIZE     = 2;
const int LINE_HEIGHT   = 22;   // spacing between lines
const int VISIBLE_LINES = 10;   // how many lines fit on the screen

// ---- Buzzer / alarm ----
bool alarmActive        = false;
unsigned long alarmEndAt = 0;
unsigned long lastToggle = 0;
const unsigned long ALARM_MS  = 5000;   // beep duration for each new msg
const unsigned long TOGGLE_MS = 250;    // beep on/off period

// ---- 5-way PRESS debounce / edge detection ----
int prevUp    = HIGH;
int prevDown  = HIGH;
int prevPress = HIGH;

unsigned long lastAckAt = 0;
const unsigned long ACK_LOCKOUT_MS = 400;

// ---- Temp buffer to assemble UART line ----
char inbuf[MAX_LINE_LEN];
int  inpos = 0;

// ===== NEW: Multi-line block state for EmergiNet =====
bool inEmergiNetBlock = false;
unsigned long lastBlockLineAt = 0;
const unsigned long BLOCK_TIMEOUT_MS = 600;  // gap that ends a block (tune if needed)

// ===== NEW: Siren state =====
bool sirenActive = false;
unsigned long sirenEndAt = 0;
unsigned long sirenLastStepAt = 0;
bool sirenHigh = false; // toggles tone freq

// ------------------------------------------------------------------
// Send "RECEIVED" message to specific Meshtastic node over Serial1
// ------------------------------------------------------------------
void sendReceivedMessage() {
  MSG_SERIAL.print("{\"FROM\":\"!9e9fc250\",\"text\":\"RECEIVED\"}\n");
}

// ===== NEW: Start siren for 15 seconds (re-triggers every time) =====
void startSiren15s() {
  sirenActive = true;
  sirenEndAt = millis() + 15000UL;
  sirenLastStepAt = 0;
  sirenHigh = false;
}

// ===== NEW: Update siren (call in loop) =====
// Uses tone() if available; if not, you can replace with your own buzzer method.
void updateSiren() {
  if (!sirenActive) return;

  unsigned long now = millis();
  if (now >= sirenEndAt) {
    sirenActive = false;
    noTone(PIN_BUZZER);
    analogWrite(PIN_BUZZER, 0);
    return;
  }

  // Toggle between two tones to simulate an emergency siren
  if (now - sirenLastStepAt >= 180) {
    sirenHigh = !sirenHigh;
    tone(PIN_BUZZER, sirenHigh ? 1200 : 700);
    sirenLastStepAt = now;
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
// Messages only, GREEN on BLACK, spacing/size like your snippet.
// ------------------------------------------------------------------
void renderScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(TEXT_SIZE);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);   // GREEN text on BLACK

  if (totalMessages == 0) {
    // Nothing to show yet
    return;
  }

  int visible = totalMessages;
  if (visible > VISIBLE_LINES) {
    visible = VISIBLE_LINES;
  }

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
// Updated behavior:
// - If a line contains "xSiren!E1", trigger siren (always).
// - Display blocks:
//    - Block starts when a line contains "EmergiNet"
//    - Once started, all consecutive lines are accepted
//    - Block ends on blank line or after timeout gap
// ------------------------------------------------------------------
void storeMessage(const char *msg) {
  if (msg == NULL) return;

  // ===== RESTORED: Siren trigger (does NOT require EmergiNet) =====
  if (strstr(msg, "xSiren!E1") != NULL) {
    startSiren15s();
    // Do NOT return here; siren can occur alongside printing rules if needed.
  }

  // If we receive a blank line, end the block and do not store it.
  if (msg[0] == '\0') {
    inEmergiNetBlock = false;
    return;
  }

  // Decide if this line should be accepted for display
  bool hasEmergiNet = (strstr(msg, "EmergiNet") != NULL);

  if (!inEmergiNetBlock) {
    // Not currently in a block: only accept if this line starts a block
    if (!hasEmergiNet) {
      // Not accepted for display; also don't start normal beep
      alarmActive = false;
      analogWrite(PIN_BUZZER, 0);
      return;
    }
    // Start new block
    inEmergiNetBlock = true;
  }

  // Accepted line (part of an EmergiNet block)
  lastBlockLineAt = millis();

  strncpy(messages[writeIndex], msg, MAX_LINE_LEN - 1);
  messages[writeIndex][MAX_LINE_LEN - 1] = '\0';

  writeIndex = (writeIndex + 1) % MAX_LINES;
  if (totalMessages < MAX_LINES) {
    totalMessages++;
  }

  int visible  = (totalMessages > VISIBLE_LINES) ? VISIBLE_LINES : totalMessages;
  int maxStart = (totalMessages > visible) ? (totalMessages - visible) : 0;
  scrollIndex  = maxStart;  // always jump to newest window

  // Normal beep for accepted EmergiNet lines (siren has priority in loop)
  alarmActive  = true;
  alarmEndAt   = millis() + ALARM_MS;
  lastToggle   = 0;

  renderScreen();
}

// ------------------------------------------------------------------
// Build lines from UART. Each nonempty '\n'-terminated line = message.
// ------------------------------------------------------------------
void handleUart() {
  while (MSG_SERIAL.available() > 0) {
    char c = MSG_SERIAL.read();

    if (c == '\r') {
      continue;   // ignore CR
    }

    if (c == '\n') {
      if (inpos > 0) {
        inbuf[inpos] = '\0';
        storeMessage(inbuf);
        inpos = 0;
      } else {
        // Blank line received -> end any active EmergiNet block
        storeMessage("");
      }
    } else {
      if (inpos < (MAX_LINE_LEN - 1)) {
        inbuf[inpos++] = c;
      }
      // extra chars beyond MAX_LINE_LEN-1 are dropped
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
  tft.setRotation(3);              // landscape

  pinMode(PIN_BUZZER, OUTPUT);
  analogWrite(PIN_BUZZER, 0);      // buzzer off

  pinMode(PIN_5S_UP,     INPUT_PULLUP);
  pinMode(PIN_5S_DOWN,   INPUT_PULLUP);
  pinMode(PIN_5S_LEFT,   INPUT_PULLUP);
  pinMode(PIN_5S_RIGHT,  INPUT_PULLUP);
  pinMode(PIN_5S_PRESS,  INPUT_PULLUP);
  pinMode(PIN_BTN_A,     INPUT_PULLUP);
  pinMode(PIN_BTN_B,     INPUT_PULLUP);

  MSG_SERIAL.begin(BAUDRATE);

    // ---- NEW: Initialize 5-way switch previous states to avoid false press on startup ----
  prevUp    = digitalRead(PIN_5S_UP);
  prevDown  = digitalRead(PIN_5S_DOWN);
  prevPress = digitalRead(PIN_5S_PRESS);

  // Optional: prime the lockout timer so even a transient won't send
  lastAckAt = millis();


  clearMessages();                 // clear buffer + screen at power-up
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

  // ===== RESTORED: Siren handler (priority) =====
  updateSiren();

// 2) Scroll with 5 way switch (PRESSSED) + send RECEIVED
// ---- Any of UP / DOWN / PRESS sends exactly one "RECEIVED" ----
int curUp    = digitalRead(PIN_5S_UP);
int curDown  = digitalRead(PIN_5S_DOWN);
int curPress = digitalRead(PIN_5S_PRESS);

// Detect a NEW press (HIGH -> LOW) on any of the three
bool newEvent =
  (prevUp    == HIGH && curUp    == LOW) ||
  (prevDown  == HIGH && curDown  == LOW) ||
  (prevPress == HIGH && curPress == LOW);

if (newEvent) {
  unsigned long now = millis();
  if (now - lastAckAt >= ACK_LOCKOUT_MS) {
    sendReceivedMessage();
    lastAckAt = now;
  }
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
      delay(300); // avoid retrigger
    }
  }

  // 5) Normal buzzer beep for new EmergiNet messages
  // Siren has priority, so do NOT run this while sirenActive
  if (!sirenActive && alarmActive) {
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
