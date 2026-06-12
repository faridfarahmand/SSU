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
 *  Updates in 1.3 version:
 *  - External siren on D2 using simple HIGH/LOW toggling (NO tone)
 *      Total: 10 seconds
 *      Pattern: 1.0 sec ON, 0.5 sec OFF
 *      Retriggerable: each "xSiren!E1" restarts the 10-second pattern
 *  - Removed old tone/noTone siren code
 *  - Added Revision number at the startup
 *  - "RECEIVED" ACK is sent as plain text (works with Meshtastic serial.mode=TEXTMSG)
 *  Updates in 1.5 version:
 *    - Message received → alert tone on D2
 *    - HELP sent → beep-beep-beep (short beeps) for 5 seconds on D2
 *    - xSiren!E1 received → rising/falling siren pattern ("doooo-dodo") for 15 seconds on D2
 *****************************************************************************************/
/*****************************************************************************************
 *  EmergiNet – Wio Terminal Meshtastic Receiver
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

#define EX_BUZ_PIN    D2   // External active buzzer/siren pin

// ---- Serial from Heltec Meshtastic ----
#define MSG_SERIAL    Serial1
#define BAUDRATE      115200

#define FWVERSION    "Version 1.5"

const char* TARGET_NODE = "!9ea2084c";

// ---- Message buffer ----
const int MAX_LINES    = 30;
const int MAX_LINE_LEN = 120;

char messages[MAX_LINES][MAX_LINE_LEN];
int totalMessages = 0;
int writeIndex    = 0;
int scrollIndex   = 0;

// ---- Message numbering ----
int messageNumber = 0;

// ---- Display layout ----
const int TEXT_SIZE     = 2;
const int LINE_HEIGHT   = 22;
const int VISIBLE_LINES = 10;

// ---- Buzzer / normal alert beep ----
bool alarmActive         = false;
unsigned long alarmEndAt = 0;
unsigned long lastToggle = 0;
const unsigned long ALARM_MS  = 5000;
const unsigned long TOGGLE_MS = 250;

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
const unsigned long BLOCK_TIMEOUT_MS = 1500;

// ===== Stronger physical-press gating =====
bool ackArmed = true;
const unsigned long PRESS_CONFIRM_MS = 35;

// ===== External buzzer patterns on D2 =====
enum ExternalBuzzerMode {
  EXT_BUZZ_OFF,
  EXT_BUZZ_SIREN,
  EXT_BUZZ_HELP
};

ExternalBuzzerMode extBuzzMode = EXT_BUZZ_OFF;

unsigned long extBuzzStartAt = 0;
unsigned long extBuzzNextAt  = 0;
int extBuzzStep = 0;

// xSiren emergency siren duration
const unsigned long EXT_SIREN_TOTAL_MS = 15000UL;

// HELP beep duration
const unsigned long HELP_BEEP_TOTAL_MS = 5000UL;

// ------------------------------------------------------------------
void sendReceivedMessage() {
  MSG_SERIAL.print("RECEIVED\n");
}

void startHelpBeeps5s();

void helpMessages() {
  MSG_SERIAL.print("HELP!\n");

  // Local audible confirmation when HELP is sent
  startHelpBeeps5s();
}

bool confirmPressLow(int pin) {
  if (digitalRead(pin) != LOW) return false;
  delay(PRESS_CONFIRM_MS);
  return (digitalRead(pin) == LOW);
}

// ------------------------------------------------------------------
// External buzzer controls on D2
// ------------------------------------------------------------------
void stopExternalBuzzer() {
  extBuzzMode = EXT_BUZZ_OFF;
  extBuzzStep = 0;
  digitalWrite(EX_BUZ_PIN, LOW);
}

void startExternalSiren15s() {
  extBuzzMode = EXT_BUZZ_SIREN;
  extBuzzStartAt = millis();
  extBuzzNextAt  = millis();
  extBuzzStep = 0;
}

void startHelpBeeps5s() {
  // Do not interrupt an active emergency siren
  if (extBuzzMode == EXT_BUZZ_SIREN) return;

  extBuzzMode = EXT_BUZZ_HELP;
  extBuzzStartAt = millis();
  extBuzzNextAt  = millis();
  extBuzzStep = 0;
}

void updateExternalBuzzer() {
  if (extBuzzMode == EXT_BUZZ_OFF) return;

  unsigned long now = millis();

  if (extBuzzMode == EXT_BUZZ_SIREN) {
    if (now - extBuzzStartAt >= EXT_SIREN_TOTAL_MS) {
      stopExternalBuzzer();
      return;
    }

    if ((long)(now - extBuzzNextAt) >= 0) {
      // Active buzzer rhythm:
      // dooooo - do-do - pause - repeat
      switch (extBuzzStep) {
        case 0:
          digitalWrite(EX_BUZ_PIN, HIGH);   // long dooooo
          extBuzzNextAt = now + 900;
          extBuzzStep = 1;
          break;

        case 1:
          digitalWrite(EX_BUZ_PIN, LOW);
          extBuzzNextAt = now + 120;
          extBuzzStep = 2;
          break;

        case 2:
          digitalWrite(EX_BUZ_PIN, HIGH);   // short do
          extBuzzNextAt = now + 160;
          extBuzzStep = 3;
          break;

        case 3:
          digitalWrite(EX_BUZ_PIN, LOW);
          extBuzzNextAt = now + 100;
          extBuzzStep = 4;
          break;

        case 4:
          digitalWrite(EX_BUZ_PIN, HIGH);   // short do
          extBuzzNextAt = now + 160;
          extBuzzStep = 5;
          break;

        case 5:
        default:
          digitalWrite(EX_BUZ_PIN, LOW);
          extBuzzNextAt = now + 350;
          extBuzzStep = 0;
          break;
      }
    }
  }

  else if (extBuzzMode == EXT_BUZZ_HELP) {
    if (now - extBuzzStartAt >= HELP_BEEP_TOTAL_MS) {
      stopExternalBuzzer();
      return;
    }

    if ((long)(now - extBuzzNextAt) >= 0) {
      // HELP rhythm:
      // beep-beep-beep, pause, repeat for 5 seconds
      switch (extBuzzStep) {
        case 0:
          digitalWrite(EX_BUZ_PIN, HIGH);
          extBuzzNextAt = now + 120;
          extBuzzStep = 1;
          break;

        case 1:
          digitalWrite(EX_BUZ_PIN, LOW);
          extBuzzNextAt = now + 100;
          extBuzzStep = 2;
          break;

        case 2:
          digitalWrite(EX_BUZ_PIN, HIGH);
          extBuzzNextAt = now + 120;
          extBuzzStep = 3;
          break;

        case 3:
          digitalWrite(EX_BUZ_PIN, LOW);
          extBuzzNextAt = now + 100;
          extBuzzStep = 4;
          break;

        case 4:
          digitalWrite(EX_BUZ_PIN, HIGH);
          extBuzzNextAt = now + 120;
          extBuzzStep = 5;
          break;

        case 5:
        default:
          digitalWrite(EX_BUZ_PIN, LOW);
          extBuzzNextAt = now + 500;
          extBuzzStep = 0;
          break;
      }
    }
  }
}

// ------------------------------------------------------------------
// Add one display line to circular buffer
// ------------------------------------------------------------------
void addDisplayLine(const char *line) {
  strncpy(messages[writeIndex], line, MAX_LINE_LEN - 1);
  messages[writeIndex][MAX_LINE_LEN - 1] = '\0';

  writeIndex = (writeIndex + 1) % MAX_LINES;

  if (totalMessages < MAX_LINES) {
    totalMessages++;
  }
}

// ------------------------------------------------------------------
// Wrap long messages so they do not overlap on the TFT
// ------------------------------------------------------------------
int maxCharsPerDisplayLine() {
  int charWidth = 6 * TEXT_SIZE;
  int maxChars = tft.width() / charWidth;

  if (maxChars < 10) maxChars = 10;
  if (maxChars > MAX_LINE_LEN - 1) maxChars = MAX_LINE_LEN - 1;

  return maxChars;
}

void addWrappedMessage(const char *text) {
  int maxChars = maxCharsPerDisplayLine();
  int len = strlen(text);
  int pos = 0;
  bool firstLine = true;

  while (pos < len) {
    char out[MAX_LINE_LEN];

    int available = maxChars;
    if (!firstLine) available -= 2;

    if (available < 5) available = 5;

    int remaining = len - pos;
    int take = (remaining > available) ? available : remaining;

    if (remaining > available) {
      int lastSpace = -1;
      for (int i = 0; i < take; i++) {
        if (text[pos + i] == ' ') lastSpace = i;
      }
      if (lastSpace > 5) {
        take = lastSpace;
      }
    }

    if (firstLine) {
      strncpy(out, text + pos, take);
      out[take] = '\0';
    } else {
      out[0] = ' ';
      out[1] = ' ';
      strncpy(out + 2, text + pos, take);
      out[take + 2] = '\0';
    }

    addDisplayLine(out);

    pos += take;

    while (text[pos] == ' ') {
      pos++;
    }

    firstLine = false;
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
  messageNumber = 0;

  tft.fillScreen(TFT_BLACK);
}

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
// ------------------------------------------------------------------
void storeMessage(const char *msg) {
  if (msg == NULL) return;

  // xSiren command: does NOT require EmergiNet
  if (strstr(msg, "xSiren!E1") != NULL) {
    startExternalSiren15s();
  }

  // HELP message received: does NOT require EmergiNet
  if (strstr(msg, "HELP") != NULL) {
    startHelpBeeps5s();
  }

  if (msg[0] == '\0') {
    inEmergiNetBlock = false;
    return;
  }

  bool hasEmergiNet = (strstr(msg, "EmergiNet") != NULL);
  bool newMessageStart = false;

  if (!inEmergiNetBlock) {
    if (!hasEmergiNet) {
      alarmActive = false;
      analogWrite(PIN_BUZZER, 0);

      if (extBuzzMode == EXT_BUZZ_OFF) {
        digitalWrite(EX_BUZ_PIN, LOW);
      }

      return;
    }

    inEmergiNetBlock = true;
    newMessageStart = true;
    messageNumber++;
  }

  lastBlockLineAt = millis();

  char numberedMsg[MAX_LINE_LEN];

  if (newMessageStart) {
    snprintf(numberedMsg, MAX_LINE_LEN, "%d-%s", messageNumber, msg);
  } else {
    snprintf(numberedMsg, MAX_LINE_LEN, "  %s", msg);
  }

  addWrappedMessage(numberedMsg);

  int visible  = (totalMessages > VISIBLE_LINES) ? VISIBLE_LINES : totalMessages;
  int maxStart = (totalMessages > visible) ? (totalMessages - visible) : 0;
  scrollIndex  = maxStart;

  // Existing received-message alert
  // This now also drives the external active buzzer on D2
  // unless xSiren or HELP pattern is currently active.
  alarmActive  = true;
  alarmEndAt   = millis() + ALARM_MS;
  lastToggle   = 0;

  renderScreen();
}

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
// Startup splash screen
// ------------------------------------------------------------------
void showSplashScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(3);

  tft.setCursor(20, 60);
  tft.println("EmergiNet");

  tft.setTextSize(2);

  tft.setCursor(20, 110);
  tft.println("Sonoma County");

  tft.setCursor(20, 150);
  tft.println(FWVERSION);

  delay(5000);

  tft.fillScreen(TFT_BLACK);
}

// ------------------------------------------------------------------
void setup() {
  tft.begin();
  tft.setRotation(3);

  showSplashScreen();

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
  digitalWrite(EX_BUZ_PIN, LOW);

  prevUp    = digitalRead(PIN_5S_UP);
  prevDown  = digitalRead(PIN_5S_DOWN);
  prevPress = digitalRead(PIN_5S_PRESS);
  lastAckAt = millis();

  MSG_SERIAL.begin(BAUDRATE);

  clearMessages();
}

// ------------------------------------------------------------------
void loop() {
  handleUart();

  if (inEmergiNetBlock) {
    unsigned long now = millis();
    if (now - lastBlockLineAt > BLOCK_TIMEOUT_MS) {
      inEmergiNetBlock = false;
    }
  }

  // Update xSiren or HELP external patterns first.
  // These have priority over normal received-message alert on D2.
  updateExternalBuzzer();

  int curUp    = digitalRead(PIN_5S_UP);
  int curDown  = digitalRead(PIN_5S_DOWN);
  int curPress = digitalRead(PIN_5S_PRESS);

  bool newEvent =
    (prevUp    == HIGH && curUp    == LOW) ||
    (prevDown  == HIGH && curDown  == LOW) ||
    (prevPress == HIGH && curPress == LOW);

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
        ackArmed = false;
      }
    }
  }

  if (!ackArmed && curUp == HIGH && curDown == HIGH && curPress == HIGH) {
    ackArmed = true;
  }

  prevUp    = curUp;
  prevDown  = curDown;
  prevPress = curPress;

  if (curUp == LOW)   scrollUp();
  if (curDown == LOW) scrollDown();

  if (digitalRead(PIN_BTN_A) == LOW) {
    delay(100);
    if (digitalRead(PIN_BTN_A) == LOW) {
      clearMessages();
      delay(300);
    }
  }

  if (digitalRead(PIN_BTN_C) == LOW) {
    delay(100);
    if (digitalRead(PIN_BTN_C) == LOW) {
      helpMessages();
      delay(300);
    }
  }

  // Existing onboard Wio buzzer alert for received EmergiNet messages.
  // Added: D2 external active buzzer follows the same alert pattern
  // unless xSiren or HELP pattern is active.
  if (alarmActive) {
    unsigned long now = millis();
    if (now < alarmEndAt) {
      if (now - lastToggle >= TOGGLE_MS) {
        static bool on = false;
        on = !on;

        analogWrite(PIN_BUZZER, on ? 200 : 0);

        if (extBuzzMode == EXT_BUZZ_OFF) {
          digitalWrite(EX_BUZ_PIN, on ? HIGH : LOW);
        }

        lastToggle = now;
      }
    } else {
      analogWrite(PIN_BUZZER, 0);
      alarmActive = false;

      if (extBuzzMode == EXT_BUZZ_OFF) {
        digitalWrite(EX_BUZ_PIN, LOW);
      }
    }
  }

  delay(5);
}
