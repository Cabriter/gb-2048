# Game Boy 2048

使用 GBDK-2020 开发的经典 2048 游戏，可在 Game Boy / Game Boy Color 上运行。

## 🎮 在线游玩

**现在可以在浏览器中直接游玩！**

访问 [GitHub Pages](https://cabriter.github.io/gb-2048) 即可在线游玩，无需下载 ROM 文件。

## 游戏截图

```
     2048 GAME
+---+---+---+---+
| . | . | 2 | . |
+---+---+---+---+
| . | . | . | . |
+---+---+---+---+
| . | 4 | . | . |
+---+---+---+---+
| . | . | . | . |
+---+---+---+---+
Score: 0
```

## 快速开始

### 操作说明

| Game Boy 按键 | SameBoy (Mac) | 功能 |
|--------------|--------------|------|
| **A** | **X** | 重新开始游戏 |
| **方向键** | **方向键** | 移动方块 |

### 游戏规则

1. 使用方向键移动所有方块
2. 相同数字的方块碰撞时会合并
3. 目标是合并出 **2048** 方块
4. 网格填满且无法合并时游戏结束
5. 按 **A 键** (SameBoy: X) 随时重新开始

## 编译要求

**GBDK-2020** - Game Boy Development Kit
- 下载：https://github.com/gbdk-2020/gbdk-2020/releases

## 编译步骤

### macOS / Linux

```bash
# 1. 修改 Makefile 中的 GBDK_HOME 路径
# 例如：GBDK_HOME = /opt/gbdk

# 2. 编译
make

# 输出：2048.gb
```

### Windows

```batch
# 1. 修改 build.bat 中的 GBDK_HOME 路径
# 例如：set GBDK_HOME=C:\gbdk

# 2. 编译
build.bat
```

## 运行游戏

### SameBoy (推荐)

```bash
# macOS
open /Applications/SameBoy.app --args 2048.gb

# Windows
SameBoy.exe 2048.gb
```

### 其他模拟器

- **mGBA**: `mgba 2048.gb`
- **OpenEmu**: 拖拽文件到窗口
- **VisualBoyAdvance**: 文件 → 打开

## 项目结构

```
gbdk-2048/
├── src/
│   ├── main.c      # 主程序（显示、输入）
│   ├── game.c      # 游戏逻辑
│   └── game.h      # 头文件
├── 2048.gb         # 编译好的 ROM
├── Makefile        # 构建脚本 (macOS/Linux)
├── build.bat       # 构建脚本 (Windows)
├── MANUAL.md       # 详细游戏说明书
└── README.md       # 本文件
```

## 技术细节

- **平台**: Game Boy / Game Boy Color
- **开发工具**: GBDK-2020
- **语言**: C
- **ROM 大小**: 32 KB
- **显示**: 背景映射 (20x18 瓦片)

## 策略提示

1. **保持角落**：将最大方块固定在角落
2. **有序排列**：按大小顺序排列方块
3. **避免分散**：小数字不要在大数字旁边
4. **谨慎重置**：A 键会立即重新开始

## 故障排除

| 问题 | 解决方法 |
|------|---------|
| 按键无反应 | 检查模拟器按键映射 (SameBoy: A=X, B=Z) |
| 画面异常 | 使用 GBDK-2020 v4.3.0+ |
| 编译失败 | 检查 GBDK 路径配置 |

## 许可证

MIT License

## 致谢

- 2048 原作者：Gabriele Cirulli
- GBDK-2020 团队
- SameBoy 模拟器

---

详细游戏说明请查看 [MANUAL.md](MANUAL.md)
