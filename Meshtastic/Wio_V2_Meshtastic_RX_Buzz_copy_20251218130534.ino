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
// ---- 5-way edge detection + lockout (shared) ----
int prevUp    = HIGH;
int prevDown  = HIGH;
int prevPress = HIGH;

unsigned long lastAckAt = 0;
const unsigned long ACK_LOCKOUT_MS = 400;



// ---- Temp buffer to assemble UART line ----
char inbuf[MAX_LINE_LEN];
int  inpos = 0;

// ------------------------------------------------------------------
// Send "RECEIVED" message to specific Meshtastic node over Serial1
// ------------------------------------------------------------------
void sendReceivedMessage() {
  MSG_SERIAL.print("{\"FROM\":\"!9e9fc250\",\"text\":\"RECEIVED\"}\n");
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
// Store a new message, auto scroll to newest, beep, redraw
// ------------------------------------------------------------------
void storeMessage(const char *msg) {
  strncpy(messages[writeIndex], msg, MAX_LINE_LEN - 1);
  messages[writeIndex][MAX_LINE_LEN - 1] = '\0';

  writeIndex = (writeIndex + 1) % MAX_LINES;
  if (totalMessages < MAX_LINES) {
    totalMessages++;
  }

  int visible  = (totalMessages > VISIBLE_LINES) ? VISIBLE_LINES : totalMessages;
  int maxStart = (totalMessages > visible) ? (totalMessages - visible) : 0;
  scrollIndex  = maxStart;  // always jump to newest window

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

  clearMessages();                 // clear buffer + screen at power-up
}

// ------------------------------------------------------------------
void loop() {
  // 1) Handle incoming messages
  handleUart();

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

  // 5) Buzzer beep for new messages
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
