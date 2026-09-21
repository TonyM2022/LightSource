#pragma once

/*
 * 驱动层：LEDC 硬件 PWM 控制 LED 输出
 */

class LedController {
public:
  void begin();          // 初始化 LEDC 并绑定引脚，默认常亮
  void setMode(int idx); // 应用 MODES[idx]：常亮或指定频率 50% 占空比
  int mode() const;      // 当前模式索引

private:
  int currentMode_ = 0;
};
