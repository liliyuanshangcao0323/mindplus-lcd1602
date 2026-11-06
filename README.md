# mindplus-lcd1602

**Mind+ 上传模式扩展**：在 BBC micro:bit V2 上用积木驱动 LCD1602 / LCD2004 字符显示屏（I2C 版）。

代码烧录进 micro:bit 后**离线独立运行**，断电不丢，不需要电脑一直连着。

---

## 一、适用设备（Important: Supported Hardware）

这个扩展**只能**用在下面这套组合上，其它主控/屏幕**不支持**：

### ✅ 支持的设备

| 类别 | 具体要求 | 说明 |
|---|---|---|
| **主控板** | **BBC micro:bit V2**（含 V2.2，nRF52833） | 必须有边缘金手指连接器 |
| 主控板 | ❌ micro:bit V1 **不支持** | V1 无 P19/P20 的 I2C 能力与本文档的引脚编号 |
| 主控板 | ❌ Arduino / ESP32 / 掌控板 **不支持** | 扩展按 micro:bit 的 CODAL 内核编写 |
| **显示屏** | **LCD1602**（2 行 × 16 字符） | 已验证 |
| 显示屏 | **LCD2004**（4 行 × 20 字符） | 驱动已支持 4 行地址，未实测 |
| **显示屏接口** | 必须带 **PCF8574 I2C 转接背板** | 4 针：VCC / GND / SDA / SCL |
| 显示屏接口 | ❌ 16 针并行直插（无 I2C 背板）**不支持** | 没有 PCF8574 芯片无法驱动 |
| I2C 地址 | **0x27**（默认）/ 0x3F / 0x20 | 积木里可选 |
| **开发软件** | **Mind+ V1.x**，模式 = **上传模式**，主控选 **micro:bit** | Python 模式不支持 |

### ⚠️ 已验证环境

```
Mind+ 版本         : Mind+ V1.x（2025 年 6 月构建）
主控              : micro:bit V2.2（DAPLink 接口固件 v0257）
微比特固件         : 由 Mind+ 上传模式编译生成（Arduino C / CODAL 内核）
LCD 模块          : "LCD 1602 module v1.0"，PCF8574 背板，地址 0x27
```

---

## 二、接线（已验证可用）

```
LCD1602 背板        micro:bit 边缘引脚
  VCC      ──────►   3V
  GND      ──────►   GND
  SDA      ──────►   P20
  SCL      ──────►   P19
```

> **注意**：micro:bit 上 P20 = nRF **P1.00**（SDA），P19 = nRF **P0.26**（SCL）。
> 用扩展板插座插屏时务必**对准第一脚**，偏移一格会把 SDA 短路到 GND，可能烧毁 micro:bit 的 I2C 引脚。

---

## 三、安装

1. 打开 Mind+，左上角模式切换为 **上传模式**
2. 主控选择 **micro:bit**
3. 左下角「扩展」→ **用户库** 标签
4. 点击 **导入本地库**，选择本仓库的 `lcd1602a-upload-V1.0.0.mpext`
5. 积木分类「LCD1602(I2C)上传版」出现即可

---

## 四、积木

| 积木 | 说明 |
|---|---|
| `LCD1602 初始化 地址 [ADDR]` | **必须最先调用**。地址可选 0x27 / 0x3F / 0x20 |
| `LCD 第[ROW]行 显示[TEXT]` | 从第 1 列开始写一行文字 |
| `LCD 第[X]列第[ROW]行 显示[TEXT]` | 指定列位置写入 |
| `LCD 清空屏幕` | 清除全部显示 |
| `LCD 背光[开/关]` | 控制背光 |

### 示例

```
LCD1602 初始化 地址 0x27
LCD 第 1 行 显示 "Hello micro:bit!"
LCD 第 2 行 显示 "Mind+ upload"
```

---

## 五、技术说明

在 Mind+ 的 micro:bit V2 Arduino/C++ 内核（基于 CODAL）上访问**边缘引脚**的 I2C，
以下写法**都不可行**，只有最后一种有效：

| 写法 | 结果 | 原因 |
|---|---|---|
| `uBit.i2c` | ❌ 无应答 | 绑定内部传感器总线（P0.26/P0.27），不是边缘引脚 |
| 全局静态 `codal::MicroBitI2C` | ❌ 启动即崩 | 在 CODAL 运行时就绪前构造 |
| `new codal::MicroBitI2C(...)` | ❌ 卡死 | 第二个总线实例与运行时冲突 |
| **`new TwoWire(P20, P19)` + `begin()`** | ✅ **有效** | Mind+ 的 Wire 库支持边缘引脚 |

另外两点：

- 上传模式的菜单文件必须命名 `_menus/microbit.json`（按主控板名），不能用 `index.json`
- `Generator.addObject(id, type, code)` 的第三个参数**只写声明部分**（如 `lcd(0x27)`），类型由第二个参数提供

---

## 六、故障排查

| 现象 | 可能原因 | 处理 |
|---|---|---|
| 全空白、无方块 | 对比度不足 | 调背板上的蓝色电位器（部分廉价模块无此器件，存在分压缺陷） |
| 只有一排方块、无字符 | 初始化未完成 / 数据线异常 | 检查 D4–D7 焊点；确认供电充足 |
| 背光不亮 | 供电不足或背光焊点开路 | 检查 3V 供电与扩展板供电能力 |
| 完全无反应 | 接线错误 | 核对 SDA→P20、SCL→P19、VCC→3V、GND→GND |
| 编译报 `can't write to cpp file` | Mind+ 编译目录缺失或无写权限 | 检查 `%LOCALAPPDATA%\DFScratch\build` 是否存在且可写 |
| 积木下拉框报「没有定义 options 值」 | 菜单文件名错误 | 必须是 `_menus/microbit.json` |

---

## 七、从源码重新打包

```
src/
├─ config.json
└─ arduinoC/
   ├─ main.ts                         积木定义
   ├─ libraries/LCD1602A/              C++ 驱动
   ├─ _menus/microbit.json             下拉菜单
   ├─ _locales/zh-cn.json              中文翻译
   └─ _images/                         图标与封面
```

把 `config.json` 与 `arduinoC/` 一并压缩为 `.mpext`（ZIP 格式，根目录含 `config.json`）即可。

---

## 八、版本

- **v1.0.0** — 改用 Wire 库构造边缘 I2C，成功点亮 LCD1602，屏幕正常显示文字（已验证）

## 许可

MIT License
