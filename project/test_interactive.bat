@echo off
echo Testing MiniDB-TestAI functionality 3...
echo.
echo This will simulate user input for option 3 (Run Automated Tests)
echo.
echo Note: This is a simple test. For full testing, run the program manually.
echo.

REM Create a simple test input file
echo 3 > test_input.txt
echo cases1.txt >> test_input.txt
echo b >> test_input.txt

echo Running program with test input...
echo ==========================================
.\minidb.exe < test_input.txt
echo ==========================================
echo Test completed.
pause
