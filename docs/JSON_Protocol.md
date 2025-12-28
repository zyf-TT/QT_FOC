# FOC系统高效JSON消息收发协议

## 版本信息
- **版本**: 2.0
- **更新日期**: 2024-01-XX
- **编码**: UTF-8
- **消息分隔**: 换行符 `\n`

---

## 目录

1. [协议概述](#协议概述)
2. [消息结构设计](#消息结构设计)
3. [接收消息格式（设备→PC）](#接收消息格式设备pc)
4. [发送消息格式（PC→设备）](#发送消息格式pc设备)
5. [消息分组定义](#消息分组定义)
6. [增量更新机制](#增量更新机制)
7. [消息示例](#消息示例)
8. [性能优化建议](#性能优化建议)

---

## 协议概述

### 设计原则

1. **分层结构**: motorA和motorB作为顶层键，参数作为嵌套对象
2. **增量更新**: 只传输变化的字段，减少通信量
3. **分组管理**: 按功能分组（basic、foc、system），支持按需查询
4. **双电机支持**: 支持单电机或双电机独立/联合控制
5. **高效通信**: 最小化消息大小，最大化信息密度

### 基本规则

- 所有消息采用UTF-8编码
- JSON消息以换行符（`\n`）结尾
- 消息中的数值字段均为浮点数（double类型）
- 布尔字段使用 `true`/`false`
- 字段名称区分大小写（建议使用camelCase）
- 支持部分字段更新（增量更新）

---

## 消息结构设计

### 消息类型

| 类型 | 值 | 说明 |
|------|-----|------|
| `data` | 数据上报 | 设备主动发送的数据 |
| `command` | 控制命令 | PC发送的控制指令 |
| `query` | 查询请求 | PC请求查询特定数据 |
| `response` | 查询响应 | 设备响应查询请求 |

### 消息分组

| 分组 | 说明 | 包含字段 |
|------|------|----------|
| `basic` | 基本参数 | speed, current, voltage, torque, temperature, angle, isRunning |
| `foc` | FOC控制参数 | id, iq, vd, vq, ia, ib, theta, omega |
| `system` | 系统参数 | dcBus, motorTemp, driverTemp, pwmDuty, encoderAngle |
| `all` | 所有参数 | 包含所有分组的所有字段 |

### 电机标识

- `motorA`: 电机A
- `motorB`: 电机B
- `both`: 两个电机（用于命令）

---

## 接收消息格式（设备→PC）

### 1. 单电机完整数据上报

**格式：**
```json
{
  "type": "data",
  "group": "all",
  "motorA": {
    "speed": 1000.0,
    "current": 2.5,
    "voltage": 24.0,
    "torque": 0.5,
    "temperature": 45.0,
    "angle": 180.0,
    "id": 1.2,
    "iq": 2.0,
    "dcBus": 24.5,
    "motorTemp": 50.0,
    "driverTemp": 40.0,
    "pwmDuty": 75.5,
    "vd": 12.0,
    "vq": 10.0,
    "ia": 1.5,
    "ib": 1.2,
    "theta": 90.0,
    "omega": 104.72,
    "encoderAngle": 180.5,
    "isRunning": true
  }
}
```

**说明：**
- `type`: 消息类型，固定为 `"data"`
- `group`: 参数分组，可选字段
- `motorA`: 电机A的数据对象，包含所有参数

### 2. 双电机数据上报

**格式：**
```json
{
  "type": "data",
  "group": "all",
  "motorA": {
    "speed": 1000.0,
    "current": 2.5,
    "voltage": 24.0,
    "isRunning": true
  },
  "motorB": {
    "speed": 1500.0,
    "current": 3.0,
    "voltage": 24.5,
    "isRunning": true
  }
}
```

**说明：**
- 同时包含 `motorA` 和 `motorB` 两个键
- 每个电机可以包含不同的字段
- 支持部分字段更新

### 3. 分组数据上报

**格式：**
```json
{
  "type": "data",
  "group": "basic",
  "motorA": {
    "speed": 1000.0,
    "current": 2.5,
    "voltage": 24.0,
    "torque": 0.5,
    "temperature": 45.0,
    "angle": 180.0,
    "isRunning": true
  }
}
```

**说明：**
- `group` 字段指定参数分组
- 只包含该分组的参数

### 4. 增量更新（推荐）

**格式：**
```json
{
  "type": "data",
  "group": "basic",
  "motorA": {
    "speed": 1050.0,
    "current": 2.6
  }
}
```

**说明：**
- 只包含变化的字段
- 接收端只更新提供的字段
- 大幅减少通信量

### 5. 双电机增量更新

**格式：**
```json
{
  "type": "data",
  "motorA": {
    "speed": 1050.0
  },
  "motorB": {
    "current": 3.2
  }
}
```

**说明：**
- 可以只更新一个电机的部分字段
- 也可以同时更新两个电机的不同字段

### 6. 查询响应

**格式：**
```json
{
  "type": "response",
  "queryId": 12345,
  "motorA": {
    "id": 1.2,
    "iq": 2.0,
    "vd": 12.0,
    "vq": 10.0
  }
}
```

**说明：**
- `type`: 固定为 `"response"`
- `queryId`: 对应查询请求的ID
- 返回查询的字段数据

---

## 发送消息格式（PC→设备）

### 1. 查询消息

#### 查询单个电机的基本参数
```json
{
  "type": "query",
  "queryId": 12345,
  "motor": "motorA",
  "group": "basic"
}
```

#### 查询单个电机的特定字段
```json
{
  "type": "query",
  "queryId": 12346,
  "motor": "motorA",
  "group": "foc",
  "fields": ["id", "iq", "vd", "vq"]
}
```

#### 查询双电机数据
```json
{
  "type": "query",
  "queryId": 12347,
  "motor": "both",
  "group": "basic"
}
```

**字段说明：**
- `type`: 固定为 `"query"`
- `queryId`: 查询ID，用于匹配响应（可选）
- `motor`: 电机标识（`motorA`, `motorB`, `both`）
- `group`: 参数分组（`basic`, `foc`, `system`, `all`）
- `fields`: 特定字段列表（可选，用于精确查询）

### 2. 控制命令

#### 单电机速度控制
```json
{
  "type": "command",
  "motor": "motorA",
  "command": "setSpeed",
  "value": 1000.0
}
```

#### 单电机电流控制
```json
{
  "type": "command",
  "motor": "motorA",
  "command": "setCurrent",
  "value": 2.5
}
```

#### 启动/停止命令
```json
{
  "type": "command",
  "motor": "motorA",
  "command": "start"
}
```

```json
{
  "type": "command",
  "motor": "motorA",
  "command": "stop"
}
```

**命令类型：**
- `start`: 启动电机
- `stop`: 停止电机
- `setSpeed`: 设置速度（需要 `value` 字段）
- `setCurrent`: 设置电流（需要 `value` 字段）
- `setPosition`: 设置位置（需要 `value` 字段）
- `setTorque`: 设置转矩（需要 `value` 字段）

### 3. 批量参数设置

#### 单电机批量设置
```json
{
  "type": "command",
  "motor": "motorA",
  "command": "setParams",
  "params": {
    "speed": 1000.0,
    "current": 2.5,
    "voltage": 24.0
  }
}
```

#### 双电机批量设置
```json
{
  "type": "command",
  "motor": "both",
  "command": "setParams",
  "motorA": {
    "speed": 1000.0,
    "current": 2.5
  },
  "motorB": {
    "speed": 1500.0,
    "current": 3.0
  }
}
```

**说明：**
- `command`: 固定为 `"setParams"`
- `params`: 单电机时使用（motorA或motorB）
- `motorA`/`motorB`: 双电机时使用（motor为both时）

### 4. 定时查询配置

```json
{
  "type": "command",
  "command": "setQueryInterval",
  "motor": "motorA",
  "group": "basic",
  "interval": 100
}
```

**说明：**
- `interval`: 查询间隔（毫秒）
- 设备将按此间隔自动发送该分组的数据

---

## 消息分组定义

### Basic组（基本参数）

| 字段名 | 类型 | 单位 | 说明 |
|--------|------|------|------|
| `speed` | double | RPM | 电机转速 |
| `current` | double | A | 总电流 |
| `voltage` | double | V | 电压 |
| `torque` | double | N·m | 转矩 |
| `temperature` | double | °C | 温度（通用） |
| `angle` | double | ° | 角度 |
| `isRunning` | bool | - | 运行状态 |

### FOC组（FOC控制参数）

| 字段名 | 类型 | 单位 | 说明 |
|--------|------|------|------|
| `id` | double | A | d轴电流 |
| `iq` | double | A | q轴电流 |
| `vd` | double | V | d轴电压 |
| `vq` | double | V | q轴电压 |
| `ia` | double | A | α轴电流（Iα） |
| `ib` | double | A | β轴电流（Iβ） |
| `theta` | double | ° | 角度θ |
| `omega` | double | rad/s | 角速度ω |

### System组（系统参数）

| 字段名 | 类型 | 单位 | 说明 |
|--------|------|------|------|
| `dcBus` | double | V | 直流母线电压 |
| `motorTemp` | double | °C | 电机温度 |
| `driverTemp` | double | °C | 驱动器温度 |
| `pwmDuty` | double | % | PWM占空比 |
| `encoderAngle` | double | ° | 编码器角度 |

---

## 增量更新机制

### 工作原理

1. **数据快照**: 接收端维护上次接收的数据快照
2. **变化检测**: 比较新数据与快照，识别变化的字段
3. **阈值过滤**: 只发送变化超过阈值的数据（可选）
4. **增量传输**: 只传输变化的字段，减少通信量

### 变化检测示例

**上次数据：**
```json
{
  "motorA": {
    "speed": 1000.0,
    "current": 2.5,
    "voltage": 24.0
  }
}
```

**当前数据：**
```json
{
  "motorA": {
    "speed": 1050.0,
    "current": 2.5,
    "voltage": 24.2
  }
}
```

**增量更新消息：**
```json
{
  "type": "data",
  "motorA": {
    "speed": 1050.0,
    "voltage": 24.2
  }
}
```

**说明：**
- `current` 未变化，不包含在消息中
- `speed` 和 `voltage` 变化，包含在消息中

### 阈值配置

可以配置变化阈值，只有变化超过阈值才发送：

```json
{
  "type": "command",
  "command": "setChangeThreshold",
  "motor": "motorA",
  "thresholds": {
    "speed": 10.0,
    "current": 0.1,
    "voltage": 0.5
  }
}
```

---

## 消息示例

### 示例1：单电机完整数据上报
```json
{
  "type": "data",
  "group": "all",
  "motorA": {
    "speed": 1500.0,
    "current": 3.2,
    "voltage": 24.5,
    "torque": 0.8,
    "temperature": 50.0,
    "angle": 270.0,
    "id": 1.5,
    "iq": 2.8,
    "dcBus": 25.0,
    "motorTemp": 55.0,
    "driverTemp": 45.0,
    "pwmDuty": 80.0,
    "vd": 15.0,
    "vq": 12.0,
    "ia": 2.0,
    "ib": 1.8,
    "theta": 135.0,
    "omega": 157.08,
    "encoderAngle": 270.5,
    "isRunning": true
  }
}
```

### 示例2：双电机增量更新
```json
{
  "type": "data",
  "motorA": {
    "speed": 1050.0
  },
  "motorB": {
    "current": 3.2,
    "voltage": 24.3
  }
}
```

### 示例3：分组数据查询
```json
{
  "type": "query",
  "queryId": 1001,
  "motor": "motorA",
  "group": "foc",
  "fields": ["id", "iq", "vd", "vq"]
}
```

### 示例4：双电机批量设置
```json
{
  "type": "command",
  "motor": "both",
  "command": "setParams",
  "motorA": {
    "speed": 1000.0,
    "current": 2.5
  },
  "motorB": {
    "speed": 1500.0,
    "current": 3.0
  }
}
```

### 示例5：定时查询配置
```json
{
  "type": "command",
  "command": "setQueryInterval",
  "motor": "motorA",
  "group": "basic",
  "interval": 100
}
```

---

## 性能优化建议

### 1. 使用增量更新
- **优势**: 减少80-90%的通信量
- **适用**: 高频数据上报场景
- **实现**: 设备端检测变化，只发送变化的字段

### 2. 分组查询
- **优势**: 按需获取数据，减少不必要的数据传输
- **适用**: 只需要特定类型参数的场景
- **实现**: 使用 `group` 和 `fields` 字段精确查询

### 3. 定时查询配置
- **优势**: 设备自动发送，减少查询命令
- **适用**: 需要定期监控的参数
- **实现**: 配置查询间隔，设备自动上报

### 4. 批量操作
- **优势**: 一次命令设置多个参数，减少消息数量
- **适用**: 需要同时设置多个参数的场景
- **实现**: 使用 `setParams` 命令，包含多个字段

### 5. 消息压缩（可选）
- **优势**: 进一步减少传输量
- **适用**: 带宽受限的场景
- **实现**: 使用gzip等压缩算法（需要设备端支持）

### 6. 数据采样
- **优势**: 降低数据频率，减少通信量
- **适用**: 不需要高频数据的场景
- **实现**: 设备端采样，按配置的频率发送

---

## 错误处理

### 错误消息格式

```json
{
  "type": "error",
  "errorCode": 1001,
  "errorMessage": "参数超出范围",
  "motor": "motorA",
  "command": "setSpeed",
  "value": 5000.0
}
```

### 常见错误码

| 错误码 | 说明 |
|--------|------|
| 1001 | 参数超出范围 |
| 1002 | 电机未初始化 |
| 1003 | 命令格式错误 |
| 1004 | 电机故障 |
| 1005 | 通信超时 |

---

## 实现建议

### 接收端（PC）

1. **数据快照管理**: 维护motorA和motorB的数据快照
2. **分层解析**: 先解析顶层结构，再解析motorA/motorB嵌套对象
3. **增量更新**: 只更新提供的字段，保持其他字段不变
4. **变化检测**: 比较新数据与快照，识别变化

### 发送端（设备）

1. **变化检测**: 检测参数变化，生成增量更新消息
2. **分组管理**: 按分组组织数据，支持按需发送
3. **定时查询**: 支持配置定时查询，自动发送数据
4. **错误处理**: 发送错误消息，包含错误码和详细信息

---

## 版本历史

- **v2.0** (2024-01-XX): 添加分层结构、增量更新、分组查询
- **v1.0** (2024-01-XX): 初始版本，基本JSON消息格式

---

## 附录

### A. 字段映射表

| JSON字段 | FOCParameters成员 | 类型 |
|----------|------------------|------|
| speed | speed | double |
| current | current | double |
| voltage | voltage | double |
| torque | torque | double |
| temperature | temperature | double |
| angle | angle | double |
| id | id | double |
| iq | iq | double |
| dcBus | dcBus | double |
| motorTemp | motorTemp | double |
| driverTemp | driverTemp | double |
| pwmDuty | pwmDuty | double |
| vd | vd | double |
| vq | vq | double |
| ia | ia | double |
| ib | ib | double |
| theta | theta | double |
| omega | omega | double |
| encoderAngle | encoderAngle | double |
| isRunning | isRunning | bool |

### B. 消息大小估算

- **完整数据（单电机）**: ~300-400字节
- **增量更新（单电机，3个字段）**: ~80-100字节
- **查询消息**: ~60-80字节
- **命令消息**: ~60-120字节

### C. 通信频率建议

- **高频参数（speed, current）**: 10-50Hz
- **中频参数（voltage, torque）**: 5-20Hz
- **低频参数（temperature）**: 1-5Hz
- **系统参数（dcBus, pwmDuty）**: 5-10Hz

