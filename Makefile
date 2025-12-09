# MiniDB-TestAI 项目编译脚本
# 作者: [你的名字]
# 版本: 1.0

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I.
LDFLAGS = -lm

# 目标文件
TARGET = minidb
BUILD_DIR = build
SRC_DIR = .

# 源文件
SRCS = main.c \
       db/csv_loader.c \
       db/parser.c \
       db/executor.c \
       db/result.c \
       test_framework/test_loader.c \
       test_framework/test_runner.c \
       test_framework/test_reporter.c \
       ai/ai_helper.c \
       utils/string_utils.c \
       utils/file_utils.c

# 对象文件
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# 默认目标
all: $(BUILD_DIR) $(TARGET) main.out

# 检测操作系统
ifeq ($(OS),Windows_NT)
    RM = del /Q
    RMDIR = rmdir /S /Q
    TARGET := $(TARGET).exe
else
    RM = rm -f
    RMDIR = rm -rf
endif

# 创建构建目录
$(BUILD_DIR):
	@echo "创建构建目录..."
ifeq ($(OS),Windows_NT)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\db mkdir $(BUILD_DIR)\db
	@if not exist $(BUILD_DIR)\test_framework mkdir $(BUILD_DIR)\test_framework
	@if not exist $(BUILD_DIR)\ai mkdir $(BUILD_DIR)\ai
	@if not exist $(BUILD_DIR)\utils mkdir $(BUILD_DIR)\utils
else
	@mkdir -p $(BUILD_DIR)/db
	@mkdir -p $(BUILD_DIR)/test_framework
	@mkdir -p $(BUILD_DIR)/ai
	@mkdir -p $(BUILD_DIR)/utils
endif

# 主目标
$(TARGET): $(OBJS)
	@echo "链接目标文件..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "编译完成: $(TARGET)"

# 生成main.out（Unix风格）
main.out: $(TARGET)
	@echo "创建main.out..."
	cp $(TARGET) main.out
	@echo "创建完成: main.out"

# 生成main.exe（Windows风格）
main.exe: $(TARGET)
	@echo "创建main.exe..."
	cp $(TARGET) main.exe
	@echo "创建完成: main.exe"

# 编译规则
$(BUILD_DIR)/%.o: %.c
	@echo "编译: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	@echo "清理构建文件..."
	$(RM) $(TARGET)
	$(RM) *.exe
	$(RM) main.out
	-$(RMDIR) $(BUILD_DIR)
	@echo "清理完成"

# 测试
test: $(TARGET)
	@echo "运行测试..."
	./$(TARGET)

# 调试版本
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# 发布版本
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# 静态分析
analyze:
	@echo "运行静态分析..."
	-scan-build make

# 内存检查
memcheck: $(TARGET)
	@echo "运行内存检查..."
	-valgrind --leak-check=full ./$(TARGET)

# 安装
install: $(TARGET)
	@echo "安装到系统路径..."
	cp $(TARGET) /usr/local/bin/
	@echo "安装完成"

# 卸载
uninstall:
	@echo "卸载..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "卸载完成"

# 帮助信息
help:
	@echo "MiniDB-TestAI 编译选项:"
	@echo "  make all       - 编译项目 (默认)"
	@echo "  make clean     - 清理构建文件"
	@echo "  make test      - 运行测试"
	@echo "  make debug     - 编译调试版本"
	@echo "  make release   - 编译发布版本"
	@echo "  make analyze   - 运行静态分析"
	@echo "  make memcheck  - 运行内存检查"
	@echo "  make install   - 安装到系统"
	@echo "  make uninstall - 卸载"
	@echo "  make main.out  - 创建main.out"
	@echo "  make main.exe  - 创建main.exe"
	@echo "  make help      - 显示此帮助信息"

# 伪目标
.PHONY: all clean test debug release analyze memcheck install uninstall help main.out main.exe

# 依赖关系
$(BUILD_DIR)/main.o: main.c \
                    db/parser.h \
                    db/executor.h \
                    db/csv_loader.h \
                    test_framework/test_runner.h \
                    ai/ai_helper.h \
                    utils/string_utils.h

$(BUILD_DIR)/db/csv_loader.o: db/csv_loader.c \
                             db/csv_loader.h \
                             db/table.h

$(BUILD_DIR)/db/parser.o: db/parser.c \
                         db/parser.h \
                         db/table.h

$(BUILD_DIR)/db/executor.o: db/executor.c \
                           db/executor.h \
                           db/table.h

$(BUILD_DIR)/db/result.o: db/result.c \
                         db/result.h \
                         db/table.h

$(BUILD_DIR)/test_framework/test_loader.o: test_framework/test_loader.c \
                                          test_framework/test_loader.h \
                                          test_framework/testcase.h

$(BUILD_DIR)/test_framework/test_runner.o: test_framework/test_runner.c \
                                          test_framework/test_runner.h \
                                          test_framework/testcase.h \
                                          db/table.h

$(BUILD_DIR)/test_framework/test_reporter.o: test_framework/test_reporter.c \
                                            test_framework/test_reporter.h \
                                            test_framework/testcase.h

$(BUILD_DIR)/ai/ai_helper.o: ai/ai_helper.c \
                            ai/ai_helper.h \
                            db/table.h

$(BUILD_DIR)/utils/string_utils.o: utils/string_utils.c \
                                  utils/string_utils.h

$(BUILD_DIR)/utils/file_utils.o: utils/file_utils.c \
                                utils/file_utils.h
