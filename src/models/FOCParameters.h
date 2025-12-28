#ifndef FOCPARAMETERS_H
#define FOCPARAMETERS_H

// FOC参数结构体
struct FOCParameters {
    double speed;        // 转速 (RPM)
    double current;     // 电流 (A)
    double voltage;     // 电压 (V)
    double torque;      // 转矩 (N·m)
    double temperature; // 温度 (°C)
    double angle;       // 角度 (°)
    double id;          // Id (A)
    double iq;          // Iq (A)
    double dcBus;       // DC Bus电压 (V)
    double motorTemp;   // 电机温度 (°C)
    double driverTemp;  // 驱动器温度 (°C)
    double pwmDuty;     // PWM占空比 (%)
    double vd;          // Vd (V)
    double vq;          // Vq (V)
    double ia;          // Iα (A)
    double ib;          // Iβ (A)
    double theta;       // θ (度)
    double omega;       // ω (RPM)
    double encoderAngle;// 编码器角度 (度)
    bool isRunning;     // 运行状态
    
    // 构造函数，初始化所有参数为0
    FOCParameters() : 
        speed(0.0), current(0.0), voltage(0.0), torque(0.0),
        temperature(0.0), angle(0.0), id(0.0), iq(0.0),
        dcBus(0.0), motorTemp(0.0), driverTemp(0.0),
        pwmDuty(0.0), vd(0.0), vq(0.0), ia(0.0), ib(0.0),
        theta(0.0), omega(0.0), encoderAngle(0.0),
        isRunning(false) {}
};

#endif // FOCPARAMETERS_H

