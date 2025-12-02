# ===================== 基础配置（可根据实际路径修改）=====================
# 编译器（保持你的 MinGW 编译器）
CXX = x86_64-w64-mingw32-g++
# 项目根目录（自动获取，无需修改）
ROOT_DIR := $(shell cd $(dir $(lastword $(MAKEFILE_LIST))) && pwd)
# 输出目录（编译产物放这里，整洁不杂乱）
BUILD_DIR := $(ROOT_DIR)/build
# 源码目录（如果后续加多个 .cpp，自动识别）
SRC_DIR := $(ROOT_DIR)/src
# DLL 存放目录（建议把所有 SDL2*.dll 放到项目根目录的 lib 文件夹，方便管理）
DLL_DIR := $(ROOT_DIR)/lib

# ===================== 编译/链接选项（复用你的配置，仅优化格式）=====================
# 头文件路径（你的原始路径，用 += 保持扩展性）
CXXFLAGS += -IE:\SDL2_MinGW\SDL2-2.32.8\x86_64-w64-mingw32\include
CXXFLAGS += -IE:\SDL2_MinGW\SDL2_image-2.8.8\x86_64-w64-mingw32\include
CXXFLAGS += -IE:\SDL2_MinGW\SDL2_mixer-2.8.1\x86_64-w64-mingw32\include
CXXFLAGS += -IE:\SDL2_MinGW\SDL2_ttf-2.24.0\x86_64-w64-mingw32\include
# C++ 标准（保留你的 C++23，若不用新特性可注释）
CXXFLAGS += -std=c++23
# 编码配置（可选启用，解决中文乱码，默认注释）
# CXXFLAGS += -fexec-charset=GBK -finput-charset=UTF-8
# 编译警告（建议启用，帮你发现潜在问题）
CXXFLAGS += -Wall -Wextra -Wshadow

# 库文件路径（复用你的原始路径）
LDFLAGS += -LE:\SDL2_MinGW\SDL2-2.32.8\x86_64-w64-mingw32\lib
LDFLAGS += -LE:\SDL2_MinGW\SDL2_image-2.8.8\x86_64-w64-mingw32\lib
LDFLAGS += -LE:\SDL2_MinGW\SDL2_mixer-2.8.1\x86_64-w64-mingw32\lib
LDFLAGS += -LE:\SDL2_MinGW\SDL2_ttf-2.24.0\x86_64-w64-mingw32\lib

# 链接库（复用你的配置，顺序不能乱）
LDLIBS += -lmingw32
LDLIBS += -lSDL2main
LDLIBS += -lSDL2
LDLIBS += -lSDL2_image
LDLIBS += -lSDL2_mixer
LDLIBS += -lSDL2_ttf

# ===================== 自动识别文件（无需手动添加新 .cpp）=====================
# 找到 src 目录下所有 .cpp 文件
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# 把 .cpp 文件对应生成 build 目录下的 .o 文件（如 src/main.cpp → build/main.o）
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
# 可执行文件路径（放到 build 目录）
TARGET := $(BUILD_DIR)/main.exe

# ===================== 目标规则（核心逻辑）=====================
# 默认目标：编译所有文件并生成可执行文件
all: $(TARGET)

# 生成可执行文件：依赖所有 .o 文件，链接时指定库路径和库
$(TARGET): $(OBJ_FILES)
	# 确保 build 目录存在（第一次编译不会报错）
	@mkdir -p $(BUILD_DIR)
	# 链接命令（复用你的配置）
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)
	# 自动复制 DLL 到可执行文件目录（解决之前的 Error -1073741515）
	@if [ -d $(DLL_DIR) ]; then \
		cp -f $(DLL_DIR)/SDL2*.dll $(BUILD_DIR)/ 2>/dev/null || echo "未找到 SDL 相关 DLL，请检查 lib 目录"; \
	fi

# 生成 .o 文件：每个 .cpp 对应一个 .o，自动找 src 目录下的文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	# 编译命令（$< 是当前 .cpp 文件，$@ 是输出的 .o 文件）
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 运行程序：依赖可执行文件，直接运行 build 目录下的 exe
run: $(TARGET)
	$(TARGET)

# 清理产物：删除 build 目录（所有编译结果一键清除）
clean:
	@rm -rf $(BUILD_DIR)
	@echo "已清理编译产物"

# 伪目标：避免目录名和目标名冲突（规范写法）
.PHONY: all run clean