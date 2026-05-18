#include <FastLED.h>

// ===== Hardware =====
#define DATA_PIN     6
#define NUM_LEDS     50
#define LED_TYPE     WS2811
#define COLOR_ORDER  RGB    // try GRB if red/green look swapped

CRGB leds[NUM_LEDS];

// ===== Timing (seconds) =====
const float CHAR_SECONDS   = 0.5;   // y: per-character on time
const float ALL_ON_SECONDS = 1.0;   // x: all-on between words
const float BLANK_SECONDS  = 0.3;   // b: blank before/after all-on

// ===== Letter -> LED mapping (PLACEHOLDER — replace with real mapping) =====
// Each letter maps to up to 2 LED indices. Use -1 for unused slot.
const int8_t letterMap[26][2] = {
  {49, 48},  // A
  {47, 46},  // B
  {45, 44},  // C
  {43, 42},  // D
  {41, 40},  // E
  {39, 38},  // F
  {37, 36},  // G
  {35, 34},  // H

  {17, 16},  // I
  {19, 18},  // J
  {21, 20},  // K
  {23, 22},  // L
  {25, 24},  // M
  {27, 26},  // N
  {29, 28},  // O
  {31, 30},  // P
  {33, 32},  // Q

  {15, 14},  // R
  {13, 12},  // S
  {11, 10},  // T
  { 9,  8},  // U
  { 7,  6},  // V
  { 5,  4},  // W
  { 3,  2},  // X
  { 1, -1},  // Y
  { 0, -1},  // Z
};

// ===== Loop words =====
const char* words[] = {
  "KOCKA",
  "URA",
  "RADIO",
  "VRATA"
};
const uint8_t NUM_WORDS = sizeof(words) / sizeof(words[0]);

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);  // max brightness always
  clearAll();
  FastLED.show();

  randomSeed(analogRead(A0));  // seed RNG from a floating analog pin
}

void loop() {
  for (uint8_t i = 0; i < NUM_WORDS; i++) {
    showMessage(words[i]);

    // Between every word: all lights light up once
    blankFor(BLANK_SECONDS);
    allOnFor(ALL_ON_SECONDS);
    blankFor(BLANK_SECONDS);
  }

  // End of loop: all lights light up three times
  for (uint8_t j = 0; j < 3; j++) {
    allOnFor(ALL_ON_SECONDS);
    blankFor(BLANK_SECONDS);
  }
}

void showMessage(const char* msg) {
  for (uint16_t i = 0; msg[i] != '\0'; i++) {
    char c = msg[i];
    if (c >= 'a' && c <= 'z') c -= 32;
    clearAll();

    if (c >= 'A' && c <= 'Z') {
      uint8_t idx = c - 'A';
      CRGB color = randomColor();
      for (uint8_t k = 0; k < 2; k++) {
        int8_t ledIdx = letterMap[idx][k];
        if (ledIdx >= 0 && ledIdx < NUM_LEDS) leds[ledIdx] = color;
      }
    }
    // unmapped char (space, punctuation, etc.) -> all blank for y seconds

    FastLED.show();
    delaySeconds(CHAR_SECONDS);
  }
}

void allOnFor(float seconds) {
  fill_solid(leds, NUM_LEDS, randomColor());
  FastLED.show();
  delaySeconds(seconds);
}

void blankFor(float seconds) {
  clearAll();
  FastLED.show();
  delaySeconds(seconds);
}

void clearAll() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

CRGB randomColor() {
  // Random hue, full saturation, full value -> vivid max-luminance color
  return CHSV(random8(), 255, 255);
}

void delaySeconds(float seconds) {
  delay((unsigned long)(seconds * 1000.0));
}
