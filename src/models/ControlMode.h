#ifndef CONTROLMODE_H
#define CONTROLMODE_H

// 运行模式枚举
enum class ControlMode {
    SpeedControl,      // 速度控制模式
    CurrentControl,    // 电流控制模式
    PositionControl,   // 位置控制模式
    TorqueControl      // 转矩控制模式
};

#endif // CONTROLMODE_H

