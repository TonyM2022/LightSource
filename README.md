# LightSource

基于 **Wifiduino32S3（ESP32-S3）** 的 WiFi 网页控制 LED 闪烁频率项目。

手机连接开发板热点后，通过浏览器页面即可切换 LED 输出模式：常亮 / 100Hz / 120Hz / 1kHz / 5kHz，占空比固定 50%，由硬件 LEDC PWM 外设生成，时序精确、不受主循环干扰。

## 硬件

| 项目 | 说明 |
|---|---|
| 主控 | Wifiduino32S3（ESP32-S3-WROOM，240MHz，8MB Flash） |
| LED 输出 | **D8 引脚 = GPIO21** |
| LED 接法 | `GPIO21 → 220~330Ω 电阻 → LED 正极 → LED 负极 → GND` |
| 通信 | WiFi SoftAP 模式，板子自建热点，无需路由器 |

> 注意：D8（GPIO21）为纯 GPIO，无 strapping / 系统功能冲突；应避免使用 GPIO45（D2）等 strapping 引脚驱动 LED，否则可能影响 Flash 供电电压选择导致无法启动。
> 1kHz 以上频率人眼不可分辨闪烁，肉眼看近似常亮，可用手机相机观察滚动条纹或用示波器验证。

## 功能模式

| 模式 | URL 参数 | 频率 | 占空比 |
|---|---|---|---|
| 常亮 | `const` | — | 100% |
| 100 Hz | `100hz` | 100 Hz | 50% |
| 120 Hz | `120hz` | 120 Hz | 50% |
| 1 kHz | `1khz` | 1000 Hz | 50% |
| 5 kHz | `5khz` | 5000 Hz | 50% |

## 项目架构

```
src/
├── main.cpp              应用层：初始化与主循环
├── config.h              配置层：引脚 / PWM 参数 / WiFi 热点 / 模式表
├── led_controller.h/.cpp 驱动层：LEDC 硬件 PWM 输出（LedController 类）
└── web_portal.h/.cpp     服务层：SoftAP 热点 + 网页控制服务（WebPortal 类）
```

```
┌─────────────── main.cpp（应用层）───────────────┐
│  setup(): led.begin() → portal.begin(led)       │
│  loop():   portal.handle()                      │
└──────────────┬──────────────────┬───────────────┘
               │                  │
┌──────────────▼─────────┐ ┌──────▼──────────────┐
│ web_portal（服务层）    │ │ led_controller      │
│ SoftAP 热点             │ │ （驱动层）           │
│ HTTP 路由 / 与 /set     │─▶ LEDC PWM 初始化     │
│ 页面生成（手机端访问）   │ │ 频率 / 占空比切换    │
└────────────────────────┘ └──────────┬──────────┘
                                      │
                            ┌─────────▼─────────┐
                            │ D8 = GPIO21 → LED │
                            └───────────────────┘
```

- **配置层**：所有可调参数集中在 `config.h`（换引脚、改热点名/密码、增减频率档位只需改这一个文件）
- **驱动层**：`LedController` 封装 LEDC 初始化与模式切换，对外只暴露 `begin() / setMode() / mode()`
- **服务层**：`WebPortal` 负责 SoftAP、HTTP 路由与页面生成，收到网页请求后调用驱动层切换模式

## 使用步骤

1. **烧录**：USB 连接开发板
   ```
   python -m platformio run -e wifiduino32s3 -t upload
   ```
2. **接线**：按上文接法将 LED 接到 D8 引脚
3. **手机控制**：
   - WiFi 连接热点 `LightSource`（密码 `12345678`）
   - 浏览器访问 `http://192.168.4.1`
   - 点击页面按钮切换模式，当前模式绿色高亮

## 环境与构建

- 平台：PlatformIO（`espressif32@6.9.0`，对应 arduino-esp32 2.0.17）
- 框架：Arduino
- 编译：
  ```
  python -m platformio run -e wifiduino32s3
  ```
- 也可使用 VSCode PlatformIO 插件打开项目，点击 Build / Upload

## 自定义

- **更换 LED 引脚**：修改 [src/config.h](src/config.h) 中的 `LED_PIN`
- **修改热点名 / 密码**：修改 `config.h` 中的 `AP_SSID / AP_PASS`
- **增减频率档位**：修改 `config.h` 中的 `MODES[]` 表，网页按钮自动跟随生成
