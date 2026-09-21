#pragma once

/*
 * 全局配置：LED 引脚 / PWM 参数 / WiFi 热点 / 可选模式表
 */

// ---------- LED 引脚与 PWM 配置 ----------
#define LED_PIN        43                     // Wifiduino32S3 D1 (TXD0) = GPIO43
#define LEDC_CHANNEL   0                      // LEDC 通道 0
#define LEDC_BITS      10                     // 10 位分辨率，duty 0~1023
#define LEDC_DUTY_HALF ((1 << LEDC_BITS) / 2) // 50% 占空比 = 512
#define LEDC_DUTY_FULL ((1 << LEDC_BITS) - 1) // 100% = 常亮

// ---------- WiFi AP 配置 ----------
static const char *AP_SSID = "LightSource";
static const char *AP_PASS = "12345678"; // WPA2 密码至少 8 位

// ---------- 可选模式 ----------
struct LedMode {
  const char *key;   // URL 参数
  const char *label; // 网页显示
  uint32_t freq;     // 频率 Hz，0 表示常亮
};

static const LedMode MODES[] = {
    {"const", "常亮",  0},
    {"100hz", "100 Hz", 100},
    {"120hz", "120 Hz", 120},
    {"1khz",  "1 kHz", 1000},
    {"5khz",  "5 kHz", 5000},
};
static const int MODE_COUNT = sizeof(MODES) / sizeof(MODES[0]);
