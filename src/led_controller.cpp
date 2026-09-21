#include "led_controller.h"

#include <Arduino.h>

#include "config.h"

void LedController::begin() {
  ledcSetup(LEDC_CHANNEL, 1000, LEDC_BITS);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL);
  setMode(0);
}

void LedController::setMode(int idx) {
  const LedMode &m = MODES[idx];
  if (m.freq == 0) {
    // 常亮：占空比拉满（频率保持不变不影响输出电平）
    ledcWrite(LEDC_CHANNEL, LEDC_DUTY_FULL);
  } else {
    ledcSetup(LEDC_CHANNEL, m.freq, LEDC_BITS);
    ledcWrite(LEDC_CHANNEL, LEDC_DUTY_HALF); // 50%
  }
  currentMode_ = idx;
  Serial.printf("[LED] 模式切换: %s\n", m.label);
}

int LedController::mode() const { return currentMode_; }
