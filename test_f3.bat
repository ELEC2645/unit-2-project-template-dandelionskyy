@echo off
echo 测试功能3 - 自动化测试
echo.
echo 3>input.txt
echo cases1.txt>>input.txt
echo b>>input.txt
echo 6>>input.txt

.\minidb.exe < input.txt

del input.txt
