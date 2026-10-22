# UE-Calculator

基于 **Unreal Engine 5.8** 的 C++ 计算器，UMG 界面，输入中缀表达式后按 `=` 求值。

核心逻辑没有用 `eval` 或第三方库，而是自己实现了**调度场算法（Shunting-yard）**做中缀转后缀，再用栈求值。

## 功能

- 四则运算 `+ - * /`，支持括号嵌套
- 小数运算（`0.5`、`3.14`）
- 负数：表达式开头（`-5`）和括号内（`(-3)`）
- 退格 `⌫`、清零 `C`
- 非法表达式统一提示「表达式非法」，不会崩溃或输出错误结果
- 除零、多个小数点、括号不配对都会拦截

## 目录结构

```
Calculator.uproject
Config/                              项目与输入配置
Source/Calculator/
  Public/UserWidget/UW_Calculator.h  控件类声明
  Private/UserWidget/UW_Calculator.cpp   全部计算逻辑
  Private/Player/                     PlayerController
  Private/Game/                       GameMode
Content/Calculator/
  Widget/BP_UW_Calculator.uasset     UI 蓝图（继承自 UUW_Calculator）
  Map/Nullmap.umap                   空场景
  Game/Player/                       Blueprint 子类
```

## 构建

需要 UE 5.8 与本机安装的 Visual Studio 工具链。

```bash
# 右键 Calculator.uproject -> Generate Visual Studio project files
# 然后编译 CalculatorEditor 目标，或用 Build.bat：
"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" ^
  CalculatorEditor Win64 Development -Project="<path>\Calculator.uproject"
```

编辑器运行时命令行编译会被拒绝（`Unable to build while Live Coding is active`），此时在编辑器内按 `Ctrl+Alt+F11` 走 Live Coding 热编译。

## 实现要点

### 中缀转后缀

`MToB()` 用调度场算法把 `"12+3*4"` 转成 `["12", "3", "4", "*", "+"]`。

操作数不是逐字符拼接的字符串——那样 `"12+34"` 会粘成 `"1234"`。而是先用 `Temp_Number` 累积，遇到运算符或括号时整块提交进 `std::vector<FString>`，保证每个 token 边界清晰。

### 一元负号

负号有两种身份，靠 `pre` 变量区分：

| 上一个字符 | `-` 的含义 |
|---|---|
| 起始哨兵 `'a'` | 一元负号（`-5`） |
| `'('` | 一元负号（`(-3)`） |
| 数字、`)` | 二元减号（`5-3`） |
| `'u'` | 非法（`--5`） |

识别成一元负号时并入操作数，形成 `"-5"` 这个 token，求值时 `Atod` 原生支持前导负号。`'u'` 是"已消费一个符号位"的哨兵，避免 `1---2` 被拼成 `--2` 后静默算出 0。

### 一个值得记的坑

`(-3)` 早期会报「表达式非法」，根因不是负号没处理，而是压栈比较时 `compareSign('-', '(')` 返回 `-1`，导致 `(` 被当成运算符**推进了后缀数组**——括号凭空消失，后缀变成 `["3","-"]`，求值时栈深不足。修法是让 `compareSign` 对 `(` 明确返回最低优先级 `0`，并保证任何运算符都能压到它上面。

### 精度

结果用 `%.10g` 输出。`%g` 默认只有 6 位有效数字，`1/3` 会显示成 `0.333333`；提到 10 位后既能保留足够精度，也能把 `0.1+0.2` 的浮点误差舍入回 `0.3`。

## 已知限制

| 写法 | 说明 |
|---|---|
| `1*-2`、`1+-2` | 运算符后不支持一元负号，需写成 `1*(-2)` |
| `-.5` | 符号位后必须紧跟数字，写成 `-0.5` |
| `-(5)`、`-(-5)` | 负号后不能直接跟括号 |
| `1.2.3` | 一个数只允许一个小数点 |

以上写法都会明确报「表达式非法」，不会算出错误结果。
