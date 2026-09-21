/*
 * Wifiduino32S3 (ESP32-S3) —— 手机 WiFi 网页控制 LED 闪烁频率
 *
 * 分层结构：
 *   config.h          配置层：引脚 / PWM 参数 / WiFi 热点 / 模式表
 *   led_controller.*  驱动层：LEDC 硬件 PWM 输出
 *   web_portal.*      服务层：SoftAP 热点 + 网页控制服务
 *   main.cpp          应用层：初始化与主循环
 *
 * 硬件接法：GPIO43 -> 220~330R 电阻 -> LED 正极 -> LED 负极 -> GND
 *
 * 注意：
 *   - D1 同时是 UART0 的 TX 引脚，LED 输出会占用该引脚，
 *     因此串口打印无法输出到 PC（烧录不受影响）。
 *   - 1kHz 以上人眼不可分辨闪烁，肉眼看近似常亮，
 *     可用手机相机对准 LED 观察滚动条纹，或用示波器验证。
 */

#include <Arduino.h>

#include "led_controller.h"
#include "web_portal.h"

static LedController led;    // 驱动层：LED 输出
static WebPortal portal;     // 服务层：WiFi + 网页

void setup() {
  Serial.begin(115200);

  led.begin();        // 初始化 LEDC，默认常亮
  portal.begin(led);  // 启动 SoftAP 与网页服务
}

void loop() {
  portal.handle();    // 处理网页请求
}
