#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === OLED Setup ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// === Pin Assignments ===
const int potTempoPin = A0;
const int potWidthPin = A1;
const int btnStartPin = 2;
const int btnResetPin = 3;
const int extResetPin = 10;  // External reset input
const int extClockPin = 11;  // New: External clock input
const int clockSourcePin = 12; // Toggle switch: HIGH = internal, LOW = external
const int outPins[6] = {4, 5, 6, 7, 8, 9};  // Clock outputs

// === Clock & State Variables ===
bool running = false;
bool useInternalClock = true;
unsigned long lastBeat = 0;
unsigned long lastExtClock = 0;
unsigned long pulseWidth = 10;
int bpm = 120;
unsigned long interval = 500;
uint32_t counters[6] = {0};
unsigned long pulseStart[6] = {0};

// === External Clock Edge Detection ===
bool lastClockState = HIGH;

// === Setup ===
void setup() {
  Serial.begin(9600);

  // OLED Init
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);

  // Pin Modes
  pinMode(potTempoPin, INPUT);
  pinMode(potWidthPin, INPUT);
  pinMode(btnStartPin, INPUT_PULLUP);
  pinMode(btnResetPin, INPUT_PULLUP);
  pinMode(extResetPin, INPUT_PULLUP);
  pinMode(extClockPin, INPUT_PULLUP);
  pinMode(clockSourcePin, INPUT_PULLUP);

  for (int i = 0; i < 6; i++) {
    pinMode(outPins[i], OUTPUT);
    digitalWrite(outPins[i], LOW);
  }
}

// === Main Loop ===
void loop() {
  unsigned long now = millis();

  // --- Check Clock Source ---
  useInternalClock = digitalRead(clockSourcePin); // HIGH = internal, LOW = external

  // --- Read Pulse Width Pot ---
  int potWidth = analogRead(potWidthPin);
  float duty = map(potWidth, 0, 1023, 10, 90) / 100.0;  // Duty cycle from 10% to 90%
  pulseWidth = interval * duty;
 

  // --- Handle Buttons ---
  if (digitalRead(btnStartPin) == LOW) {
    delay(50);
    if (digitalRead(btnStartPin) == LOW) {
      running = !running;
    }
    while (digitalRead(btnStartPin) == LOW);
  }

  if (digitalRead(btnResetPin) == LOW) {
    delay(50);
    if (digitalRead(btnResetPin) == LOW) {
      resetCounters();
    }
    while (digitalRead(btnResetPin) == LOW);
  }

  if (digitalRead(extResetPin) == LOW) {
    resetCounters();
    while (digitalRead(extResetPin) == LOW);
  }

  // === CLOCK HANDLING ===

  if (running) {
    if (useInternalClock) {
      // --- Read Tempo Pot ---
      int potTempo = analogRead(potTempoPin);
      bpm = map(potTempo, 0, 1023, 20, 480);
      interval = 60000UL / bpm;

      // --- Internal Clock Pulse ---
      if (now - lastBeat >= interval) {
        lastBeat = now;
        triggerDivisions(now);
      }

    } else {
      // --- External Clock Handling (rising edge detection) ---
      bool currentClockState = digitalRead(extClockPin);

      if (lastClockState == HIGH && currentClockState == LOW) {
        // RISING edge detected (assuming active LOW pulses)
        unsigned long clockInterval = now - lastExtClock;
        if (clockInterval > 10 && clockInterval < 2000) { // filter noise
          interval = clockInterval;
          bpm = 60000UL / interval;
        }
        lastExtClock = now;
        lastBeat = now;
        triggerDivisions(now);
      }

      lastClockState = currentClockState;
    }
  }

  // === Turn Off Pulses After Pulse Width ===
  for (int i = 0; i < 6; i++) {
    if (digitalRead(outPins[i]) == HIGH && (now - pulseStart[i] >= pulseWidth)) {
      digitalWrite(outPins[i], LOW);
    }
  }

  // === OLED Display ===
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("BPM:");
  oled.setCursor(0, 20);
  oled.print(bpm);
  oled.setCursor(0, 45);
  oled.print(running ? (useInternalClock ? "<Int Run>" : "<Ext Run>") : "<Pause>");
  oled.display();
}

// === Trigger Clock Divisions ===
void triggerDivisions(unsigned long now) {
  for (int i = 0; i < 6; i++) {
    counters[i]++;
    uint32_t div = (1UL << i);
    if (counters[i] >= div) {
      counters[i] = 0;
      digitalWrite(outPins[i], HIGH);
      pulseStart[i] = now;
    }
  }
}

// === Reset Division Counters ===
void resetCounters() {
  for (int i = 0; i < 6; i++) {
    counters[i] = 0;
    digitalWrite(outPins[i], LOW);
  }
}
