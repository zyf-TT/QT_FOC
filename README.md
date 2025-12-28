# QT_FOC

Qt FOC（磁场定向控制）上位机控制面板 - 用于FOC电机控制系统的PC端监控和控制软件。

## 项目简介

QT_FOC是一个基于Qt框架开发的FOC电机控制系统上位机软件，通过串口与FOC控制器进行通信，提供实时数据监控、参数调节、波形显示等功能。支持双电机独立控制和联合控制模式。

## 功能特性

### 核心功能
- **双电机控制**：支持MotorA和MotorB独立控制和监控
- **实时数据监控**：实时显示电机转速、电流、电压、转矩、温度等参数
- **波形显示**：实时波形图表展示电机运行状态
- **参数调优**：提供友好的参数调节界面，支持PID参数配置
- **串口通信**：自动检测串口，支持多种波特率配置
- **多页面导航**：采用标签页设计，功能模块清晰

### 页面功能
- **仪表盘（Dashboard）**：系统概览和关键参数展示
- **电机A页面（MotorA）**：电机A的详细控制和监控
- **电机B页面（MotorB）**：电机B的详细控制和监控
- **波形页面（Waveform）**：实时波形数据可视化
- **调参页面（Tuning）**：FOC参数和PID参数调节
- **系统页面（System）**：系统设置和配置

## 技术栈

- **框架**：Qt 6.x
- **模块**：
  - Qt Widgets（GUI界面）
  - Qt SerialPort（串口通信）
- **构建系统**：CMake 3.16+
- **编程语言**：C++17
- **通信协议**：JSON（基于换行符分隔）

## 项目结构

```
QT_FOC/
├── CMakeLists.txt          # CMake构建配置
├── main.cpp                # 程序入口
├── src/
│   ├── core/               # 核心功能模块
│   │   ├── SerialPortManager.*  # 串口管理
│   │   └── DataParser.*          # 数据解析
│   ├── models/             # 数据模型
│   │   ├── FOCParameters.h       # FOC参数结构
│   │   └── ControlMode.h          # 控制模式枚举
│   ├── ui/                 # UI界面层
│   │   ├── MainWindow.*          # 主窗口
│   │   ├── pages/                # 页面组件
│   │   │   ├── DashboardPage.*
│   │   │   ├── MotorAPage.*
│   │   │   ├── MotorBPage.*
│   │   │   ├── WaveformPage.*
│   │   │   ├── TuningPage.*
│   │   │   └── SystemPage.*
│   │   ├── widgets/              # 自定义控件
│   │   │   ├── Card.*
│   │   │   ├── TopBar.*
│   │   │   ├── NavBar.*
│   │   │   ├── MotorStatusCard.*
│   │   │   ├── ElectricalStatusCard.*
│   │   │   ├── ControlOutputCard.*
│   │   │   ├── FOCDebugCard.*
│   │   │   └── StatusBadge.*
│   │   └── styles/               # 样式表
│   │       └── StyleSheet.*
│   └── utils/              # 工具类（预留）
└── docs/                   # 文档
    ├── JSON_Protocol.md         # JSON协议文档
    └── JSON_Message_Format.md   # 消息格式文档
```

## 构建说明

### 环境要求

- **Qt版本**：Qt 6.x（推荐Qt 6.10.1或更高版本）
- **CMake版本**：3.16或更高
- **编译器**：
  - Windows: MinGW-w64 (llvm-mingw) 或 MSVC
  - Linux: GCC 7+ 或 Clang 5+
  - macOS: Clang（Xcode）
- **开发工具**：Qt Creator（推荐）或支持CMake的IDE

### 构建步骤

#### 使用Qt Creator

1. 打开Qt Creator
2. 选择 `文件` -> `打开文件或项目`
3. 选择项目根目录下的 `CMakeLists.txt`
4. 配置构建套件（Kit），选择Qt 6.x版本
5. 点击 `配置项目`
6. 点击 `构建` -> `构建项目`

#### 使用命令行（Windows）

```bash
# 创建构建目录
mkdir build
cd build

# 配置CMake（替换为你的Qt安装路径）
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/llvm_mingw_64" -G "Ninja"

# 编译
cmake --build .
```

#### 使用命令行（Linux/macOS）

```bash
# 创建构建目录
mkdir build
cd build

# 配置CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
make -j4
```

### 运行

构建完成后，可执行文件位于构建目录中：
- Windows: `build/Desktop_Qt_6_10_1_llvm_mingw_64_bit-Debug/FOC.exe`
- Linux: `build/FOC`
- macOS: `build/FOC.app`

## 使用说明

### 连接设备

1. **连接串口**：
   - 使用USB转串口线连接FOC控制器到PC
   - 确保驱动程序已正确安装

2. **打开软件**：
   - 运行FOC.exe（Windows）或FOC（Linux/macOS）

3. **配置串口**：
   - 在顶部工具栏选择串口号
   - 选择波特率（通常为115200或9600）
   - 点击"连接"按钮

4. **开始监控**：
   - 连接成功后，软件会自动接收设备数据
   - 在各个页面查看实时参数和波形

### 控制电机

- **启动/停止**：使用控制面板上的启动/停止按钮
- **参数调节**：在调参页面修改FOC参数和PID参数
- **模式切换**：选择速度控制、电流控制、位置控制或转矩控制模式

### 数据监控

- **实时数据**：在仪表盘和电机页面查看实时参数
- **波形显示**：在波形页面查看实时波形图
- **调试信息**：在FOC调试卡片中查看详细的调试数据

## 通信协议

本软件通过串口与FOC控制器通信，使用JSON格式的消息协议。详细协议文档请参考：

- [JSON协议文档](docs/JSON_Protocol.md) - 完整的协议说明
- [消息格式文档](docs/JSON_Message_Format.md) - 消息格式详细说明

### 协议特点

- **消息格式**：JSON，UTF-8编码
- **消息分隔**：换行符（`\n`）
- **增量更新**：支持部分字段更新，减少通信量
- **双电机支持**：支持MotorA和MotorB独立控制
- **分组管理**：按功能分组（basic、foc、system）

## 开发说明

### 代码结构

- **core模块**：核心功能，包括串口管理和数据解析
- **models模块**：数据模型定义
- **ui模块**：用户界面，采用MVC架构
  - `pages`：各个功能页面
  - `widgets`：可复用的UI组件
  - `styles`：样式表定义

### 添加新功能

1. **添加新页面**：
   - 在 `src/ui/pages/` 创建新的页面类
   - 在 `MainWindow` 中注册页面
   - 在 `NavBar` 中添加导航项

2. **添加新控件**：
   - 在 `src/ui/widgets/` 创建控件类
   - 继承自 `Card` 或 `QWidget`

3. **扩展数据模型**：
   - 在 `src/models/` 添加新的数据结构
   - 更新 `DataParser` 以支持新数据格式

## 许可证

本项目采用私有许可证，仅供内部使用。

## 更新日志

### v0.1
- 初始版本
- 实现基本的双电机监控和控制功能
- 支持串口通信和JSON协议
- 完成多页面UI界面

## 联系方式

如有问题或建议，请联系开发团队。

---

**注意**：使用本软件前，请确保FOC控制器固件版本与协议文档兼容。
