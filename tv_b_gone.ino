/*
 * TV-B-Gone + IR Jammer для ESP32-C3 SuperMini
 *
 * GPIO9  → IR модуль (DATA)
 * GPIO3  → внешняя кнопка TV-B-Gone → GND
 * GPIO9  → кнопка Boot (встроенная) — используем как вход
 * GPIO8  → встроенный LED
 *
 * Логика:
 *   Включился        → ничего, ждёт
 *   Клик Boot        → джам бесконечно (LED горит)
 *   Клик GPIO3       → джам стоп, TV-B-Gone (LED мигает), потом ждёт
 *   Reset            → всё останавливает
 */

#include "codes.h"

#define IR_PIN   9   // IR выход И кнопка Boot — но Boot читается через BOOT_PIN
#define BOOT_PIN 9   // На ESP32-C3 кнопка Boot подтянута к 3.3V, нажатие = LOW
#define BTN_PIN  3   // Внешняя кнопка
#define LED_PIN  8   // LOW = вкл

// ПРОБЛЕМА: GPIO9 одновременно IR и Boot — нельзя!
// Решение: IR на GPIO9, Boot читаем отдельно через аппаратный регистр
// На ESP32-C3 SuperMini кнопка Boot висит на GPIO9
// Поэтому IR переносим на GPIO10, Boot остаётся на GPIO9

#undef IR_PIN
#define IR_PIN  10  // IR модуль подключить на GPIO10
                    // Boot кнопка на GPIO9 — только вход

bool jamming = false;

void IRAM_ATTR irHigh() { digitalWrite(IR_PIN, HIGH); }
void IRAM_ATTR irLow()  { digitalWrite(IR_PIN, LOW);  }

void IRAM_ATTR sendMark(uint16_t us) {
  uint32_t end = micros() + us;
  while ((int32_t)(end - micros()) > 0) {
    digitalWrite(IR_PIN, HIGH);
    delayMicroseconds(13);
    digitalWrite(IR_PIN, LOW);
    delayMicroseconds(13);
  }
}

void IRAM_ATTR sendSpace(uint16_t us) {
  digitalWrite(IR_PIN, LOW);
  delayMicroseconds(us);
}

void sendCode(const IrCode &code) {
  for (uint8_t repeat = 0; repeat < code.nRepeat + 1; repeat++) {
    bool isMark = true;
    for (uint16_t i = 0; i < code.nPairs; i++) {
      uint8_t timingIdx = pgm_read_byte(&code.pairs[i]);
      uint16_t duration = pgm_read_word(&code.timings[timingIdx]);
      if (isMark) sendMark(duration);
      else        sendSpace(duration);
      isMark = !isMark;
    }
    if (repeat < code.nRepeat) sendSpace(code.gap);
  }
  delay(100);
}

void runJam() {
  Serial.println("ДЖАМ запущен");
  jamming = true;
  digitalWrite(LED_PIN, LOW); // LED горит

  while (jamming) {
    // ~1мс несущей
    uint32_t end = micros() + 1000;
    while ((int32_t)(end - micros()) > 0) {
      digitalWrite(IR_PIN, HIGH);
      delayMicroseconds(13);
      digitalWrite(IR_PIN, LOW);
      delayMicroseconds(13);
    }
    // Проверяем кнопку GPIO3
    if (digitalRead(BTN_PIN) == LOW) {
      delay(50);
      if (digitalRead(BTN_PIN) == LOW) {
        jamming = false;
      }
    }
  }

  digitalWrite(IR_PIN, LOW);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("ДЖАМ остановлен");
  while (digitalRead(BTN_PIN) == LOW) delay(10); // ждём отпускания
}

void runTVBGone() {
  Serial.println("TV-B-Gone запущен");

  for (uint16_t i = 0; i < NUM_CODES; i++) {
    Serial.printf("Код %u / %u\n", i + 1, NUM_CODES);
    sendCode(allCodes[i]);

    digitalWrite(LED_PIN, LOW);
    delay(50);
    digitalWrite(LED_PIN, HIGH);
  }

  Serial.println("TV-B-Gone завершён");
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN,  OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(BOOT_PIN, INPUT_PULLUP); // Boot кнопка
  digitalWrite(IR_PIN,  LOW);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Готов. Boot = джам, GPIO3 = TV-B-Gone");
}

void loop() {
  // Ждём нажатия Boot (GPIO9 = LOW)
  if (digitalRead(BOOT_PIN) == LOW) {
    delay(50);
    if (digitalRead(BOOT_PIN) == LOW) {
      while (digitalRead(BOOT_PIN) == LOW) delay(10); // ждём отпускания
      runJam(); // джам до нажатия GPIO3
    }
  }

  // Ждём нажатия GPIO3
  if (digitalRead(BTN_PIN) == LOW) {
    delay(50);
    if (digitalRead(BTN_PIN) == LOW) {
      while (digitalRead(BTN_PIN) == LOW) delay(10);
      runTVBGone();
    }
  }
}
