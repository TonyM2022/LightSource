#include "web_portal.h"

#include <Arduino.h>
#include <WiFi.h>

#include "config.h"

void WebPortal::begin(LedController &led) {
  led_ = &led;

  // SoftAP 模式：板子自建热点，手机无需路由器即可连接
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("[WiFi] AP IP: ");
  Serial.println(WiFi.softAPIP()); // 固定为 192.168.4.1

  server_.on("/", [this] { handleRoot(); });
  server_.on("/set", [this] { handleSet(); });
  server_.begin();
  Serial.println("[Web] 服务已启动: http://192.168.4.1");
}

void WebPortal::handle() { server_.handleClient(); }

void WebPortal::handleRoot() { server_.send(200, "text/html", buildPage()); }

void WebPortal::handleSet() {
  String mode = server_.arg("mode");
  for (int i = 0; i < MODE_COUNT; i++) {
    if (mode == MODES[i].key) {
      led_->setMode(i);
      break;
    }
  }
  server_.sendHeader("Location", "/");
  server_.send(302, "text/plain", "");
}

// 生成控制页面（手机端访问，当前模式高亮）
String WebPortal::buildPage() const {
  String html = "<!DOCTYPE html><html><head>"
                "<meta charset='utf-8'>"
                "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                "<title>LED 频率控制</title><style>"
                "body{font-family:sans-serif;text-align:center;background:#111;color:#eee;margin:0;}"
                "h1{font-size:1.4em;margin:24px 8px 4px;}"
                ".btn{display:block;width:82%;max-width:320px;margin:12px auto;padding:16px;"
                "font-size:1.1em;border-radius:12px;border:1px solid #555;background:#222;"
                "color:#eee;text-decoration:none;}"
                ".btn.active{background:#0a7d32;border-color:#0a7d32;font-weight:bold;}"
                "p{color:#888;font-size:0.85em;}"
                "</style></head><body>"
                "<h1>LED 闪烁频率控制</h1>"
                "<p>占空比固定 50%</p>";
  for (int i = 0; i < MODE_COUNT; i++) {
    html += "<a class='btn";
    if (i == led_->mode()) {
      html += " active";
    }
    html += "' href='/set?mode=";
    html += MODES[i].key;
    html += "'>";
    html += MODES[i].label;
    html += "</a>";
  }
  html += "<p>连接热点 ";
  html += AP_SSID;
  html += " 后访问 192.168.4.1</p></body></html>";
  return html;
}
