#pragma once

#include <WebServer.h>

#include "led_controller.h"

/*
 * 服务层：SoftAP 热点 + 网页控制服务
 */

class WebPortal {
public:
  void begin(LedController &led); // 启动 SoftAP 与 HTTP 服务
  void handle();                  // 主循环调用，处理网页请求

private:
  String buildPage() const; // 生成控制页面
  void handleRoot();
  void handleSet();

  WebServer server_{80};
  LedController *led_ = nullptr;
};
