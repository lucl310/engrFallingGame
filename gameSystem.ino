#include <Adafruit_CircuitPlayground.h>
#include <Arduino.h>
// #include <Adafruit_NeoPixel.h>

// #define PIXEL_PIN    A1
// #define PIXEL_COUNT  10

const int LEVEL_DURATION = 15000;
const int INITIAL_FALL_DELAY = 1000;
const int LEVEL_DELAY_DECREMENT = 100;
const int BUTTON_DEBOUNCE_DELAY = 50;

const uint32_t OFF_COLOR = 0x000000;
const uint32_t HIT_COLOR = (0, 255, 0);
const uint32_t MISS_COLOR = (255, 0, 0);
const uint32_t LEVEL_COLORS[] = {(0, 0, 255), (255, 255, 0), (128, 0, 128), (255, 165, 0)};

const int LEFT_BUTTON_PIN = A1;
const int RIGHT_BUTTON_PIN = A2;

const int LEFT_LED_PIN = 5;
const int RIGHT_LED_PIN = 6;

uint32_t CURRENT_LEFT_COLOR;
uint32_t CURRENT_RIGHT_COLOR;

int level = 1;
int score = 0;
unsigned long levelStartTime = 0;
unsigned long lastFallTime = 0;
unsigned long fallDelay = INITIAL_FALL_DELAY;
bool gameStarted = false;


void setup() {
  CircuitPlayground.begin();

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON_PIN), handleButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON_PIN), handleButtonPress, FALLING);

  startGame();
}

void loop() {
  if (gameStarted) {
    unsigned long currentTime = millis();

    if (currentTime - levelStartTime >= LEVEL_DURATION) {
      level++;
      levelStartTime = currentTime;
      fallDelay -= LEVEL_DELAY_DECREMENT;
      if (level > sizeof(LEVEL_COLORS) / sizeof(LEVEL_COLORS[0])) {
        level = sizeof(LEVEL_COLORS) / sizeof(LEVEL_COLORS[0]);
      }

      CircuitPlayground.setPixelColor(LEFT_LED_PIN, LEVEL_COLORS[level - 1]);
      CircuitPlayground.setPixelColor(RIGHT_LED_PIN, LEVEL_COLORS[level - 1]);
        CURRENT_LEFT_COLOR = LEVEL_COLORS[level - 1];
        CURRENT_LEFT_COLOR = LEVEL_COLORS[level - 1];
    }

    if (currentTime - lastFallTime >= fallDelay) {
      CircuitPlayground.setPixelColor(LEFT_LED_PIN, OFF_COLOR);
      CircuitPlayground.setPixelColor(RIGHT_LED_PIN, OFF_COLOR);
      CURRENT_LEFT_COLOR = OFF_COLOR;
      CURRENT_RIGHT_COLOR = OFF_COLOR;

      int randomLed = random(2);
      if (randomLed == 0) {
        CircuitPlayground.setPixelColor(LEFT_LED_PIN, LEVEL_COLORS[level - 1]);
        CURRENT_LEFT_COLOR = LEVEL_COLORS[level - 1];
      } else {
        CircuitPlayground.setPixelColor(RIGHT_LED_PIN, LEVEL_COLORS[level - 1]);
        CURRENT_RIGHT_COLOR = LEVEL_COLORS[level - 1];
      }

      lastFallTime = currentTime;
    }

    if (currentTime - lastFallTime >= fallDelay * 2) {
      gameStarted = false;
      if (CURRENT_LEFT_COLOR != HIT_COLOR) {
        CircuitPlayground.setPixelColor(LEFT_LED_PIN, MISS_COLOR);
        CURRENT_LEFT_COLOR = MISS_COLOR;
      }
      if (CURRENT_RIGHT_COLOR != HIT_COLOR) {
        CircuitPlayground.setPixelColor(RIGHT_LED_PIN, MISS_COLOR);
        CURRENT_RIGHT_COLOR = MISS_COLOR;
      }
    }
  }

  delay(BUTTON_DEBOUNCE_DELAY);
}

void startGame() {
  level = 1;
  score = 0;
  levelStartTime = 0;
  lastFallTime = 0;
  fallDelay = INITIAL_FALL_DELAY;
  gameStarted = true;

  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(LEFT_LED_PIN, LEVEL_COLORS[level - 1]);
  CURRENT_LEFT_COLOR = LEVEL_COLORS[level - 1];
  CircuitPlayground.setPixelColor(RIGHT_LED_PIN, LEVEL_COLORS[level - 1]);
  CURRENT_RIGHT_COLOR = LEVEL_COLORS[level - 1];
}

void handleButtonPress() {
  if (gameStarted) {
    unsigned long currentTime = millis();

    if (CircuitPlayground.leftButton()) {
      CircuitPlayground.setPixelColor(LEFT_LED_PIN, HIT_COLOR);
        CURRENT_LEFT_COLOR = HIT_COLOR;
      if (currentTime - lastFallTime <= fallDelay) {
        score++;
      } else {
        gameStarted = false;
        CircuitPlayground.setPixelColor(RIGHT_LED_PIN, MISS_COLOR);
        CURRENT_RIGHT_COLOR = MISS_COLOR;
      }
    }

    if (CircuitPlayground.rightButton()) {
      CircuitPlayground.setPixelColor(RIGHT_LED_PIN, HIT_COLOR);
        CURRENT_RIGHT_COLOR = HIT_COLOR;
      if (currentTime - lastFallTime <= fallDelay) {
        score++;
      } else {
        gameStarted = false;
        CircuitPlayground.setPixelColor(LEFT_LED_PIN, MISS_COLOR);
        CURRENT_LEFT_COLOR = MISS_COLOR;
      }
    }
  }
}