# GBDK-2020 Makefile for 2048 Game Boy
#
# 使用前请修改 GBDK_HOME 为你的 GBDK-2020 安装路径
# 例如：
#   macOS:     GBDK_HOME = /opt/gbdk
#   Windows:   GBDK_HOME = C:/gbdk
#   主目录：   GBDK_HOME = $(HOME)/gbdk

GBDK_HOME = /Users/cabrite/Desktop/WorkSpace/gbdk
LCC = $(GBDK_HOME)/bin/lcc

# 编译标志
LCCFLAGS += -Wl-yt0x1B -Wl-j -Wm-yoA -Wm-ya4 -autobank -Wb-ext=.rel -Wb-v

# 项目设置
PROJECTNAME = 2048
SRCDIR = src
OBJDIR = obj
BINDIR = .

# 源文件
OBJS = $(OBJDIR)/main.o $(OBJDIR)/game.o

# 默认目标
all: $(BINDIR)/$(PROJECTNAME).gb

# 编译 .c 到 .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(LCC) -msm83:gb -c -o $@ $<

# 链接 .o 到 .gb
$(BINDIR)/$(PROJECTNAME).gb: $(OBJS)
	$(LCC) $(LCCFLAGS) -msm83:gb -o $@ $(OBJS)

# 清理
clean:
	rm -rf $(OBJDIR)
	rm -f $(BINDIR)/$(PROJECTNAME).gb
	rm -f $(BINDIR)/$(PROJECTNAME).ihx
	rm -f $(BINDIR)/$(PROJECTNAME).map
	rm -f $(BINDIR)/$(PROJECTNAME).noi

# 运行 (SameBoy)
run: $(BINDIR)/$(PROJECTNAME).gb
	open /Applications/SameBoy.app --args $(BINDIR)/$(PROJECTNAME).gb

# 帮助
help:
	@echo "GBDK 2048 Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all   - 编译游戏 (默认)"
	@echo "  clean - 清理编译文件"
	@echo "  run   - 用 SameBoy 运行游戏"
	@echo "  help  - 显示此帮助信息"
	@echo ""
	@echo "使用前请修改 Makefile 中的 GBDK_HOME 路径"

.PHONY: all clean run help
