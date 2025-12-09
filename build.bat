@echo off
echo 编译 MiniDB-TestAI 项目...

echo 创建构建目录...
if not exist build mkdir build
if not exist build\db mkdir build\db
if not exist build\test_framework mkdir build\test_framework
if not exist build\ai mkdir build\ai
if not exist build\utils mkdir build\utils

echo 编译源文件...
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/main.c -o build/main.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/db/csv_loader.c -o build/db/csv_loader.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/db/parser.c -o build/db/parser.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/db/executor.c -o build/db/executor.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/db/result.c -o build/db/result.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/test_framework/test_loader.c -o build/test_framework/test_loader.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/test_framework/test_runner.c -o build/test_framework/test_runner.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/test_framework/test_reporter.c -o build/test_framework/test_reporter.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/ai/ai_helper.c -o build/ai/ai_helper.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/utils/string_utils.c -o build/utils/string_utils.o
gcc -Wall -Wextra -std=c99 -g -Isrc -c src/utils/file_utils.c -o build/utils/file_utils.o

echo 链接目标文件...
gcc build/main.o ^
    build/db/csv_loader.o ^
    build/db/parser.o ^
    build/db/executor.o ^
    build/db/result.o ^
    build/test_framework/test_loader.o ^
    build/test_framework/test_runner.o ^
    build/test_framework/test_reporter.o ^
    build/ai/ai_helper.o ^
    build/utils/string_utils.o ^
    build/utils/file_utils.o ^
    -o minidb.exe -lm

echo 复制可执行文件...
copy minidb.exe main.out > nul
copy minidb.exe main.exe > nul

echo 编译完成: minidb.exe, main.out, main.exe
