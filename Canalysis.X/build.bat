@echo off
REM Build script for CANalysis firmware
REM Builds the project and copies to network share for auto-import

echo ========================================
echo   CANalysis Build Script
echo ========================================
echo.

cd /d "%~dp0"

echo Building...
"C:\Program Files\Microchip\MPLABX\v6.20\gnuBins\GnuWin32\bin\make.exe" -f nbproject/Makefile-default.mk CONF=default

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ========================================
    echo   BUILD FAILED!
    echo ========================================
    pause
    exit /b 1
)

echo.
echo Build successful!
echo.

echo Copying to network share...
copy /Y "dist\default\production\Canalysis.X.production.hex" "\\HOMARBGNAS01\home\Temp\Canalysis.X.production.hex"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo WARNING: Failed to copy to network share
    echo The hex file is still available at:
    echo   dist\default\production\Canalysis.X.production.hex
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   BUILD COMPLETE - Copied to network share
echo ========================================
echo.
echo Hex file: \\HOMARBGNAS01\home\Temp\Canalysis.X.production.hex
echo.
pause

