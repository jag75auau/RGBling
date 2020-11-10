if "%~1"=="" goto usage
set comp=%1

:start
cls
@echo off
echo           RGBling
echo.
echo    [q] Quit
echo    [n] None
echo    [0] DigitalRGB
echo    [1] Wave
echo    [2] Spectrum
echo    [3] Static
echo    [4] Supercar
echo    [5] Flux
echo    [6] Wipe
echo    [7] Fade
echo    [8] Running
echo    [9] Meteor
echo.
    
echo.
set /P _mode= Enter a number between 0 and 9 for modes or N for off:
if "%_mode%"=="q" goto end
if "%_mode%"=="n" goto none
if "%_mode%"=="0" goto digitalrgb
if "%_mode%"=="1" goto wave
if "%_mode%"=="2" goto spectrum
if "%_mode%"=="3" goto static
if "%_mode%"=="4" goto supercar
if "%_mode%"=="5" goto flux
if "%_mode%"=="6" goto wipe
if "%_mode%"=="7" goto fade
if "%_mode%"=="8" goto running
if "%_mode%"=="9" goto meteor

:none
echo none > %comp%
goto start

:digitalrgb
echo digitalrgb > %comp%
goto start

:wave
echo wave > %comp%
goto start

:spectrum
echo spectrum > %comp%
goto start

:static
echo.
set /P _numcolour= Enter the number of colours 1 to 3:
if "%_numcolour"=="1" goto :static1
if "%_numcolour"=="2" goto :static2
if "%_numcolour"=="3" goto :static3
goto start

:static1
echo.
set /P _red= Enter the RED value between 000 to 255:
set /P _green= Enter GREEN value between 000 to 255:
set /P _blue= Enter BLUE value between 000 to 255:
echo static %_red% %_green% %_blue% > %comp%
goto start

:static2
echo.
echo Colour 1
set /P _red1= Enter the RED value between 000 to 255:
set /P _green1= Enter GREEN value between 000 to 255:
set /P _blue1= Enter BLUE value between 000 to 255:
echo.
echo Colour 2
set /P _red2= Enter the RED value between 000 to 255:
set /P _green2= Enter GREEN value between 000 to 255:
set /P _blue2= Enter BLUE value between 000 to 255:
echo static %_red1% %_green1% %_blue1% %_red2% %_green2% %_blue2% > %comp%
goto start

:static3
echo.
echo Colour 1
set /P _red1= Enter the RED value between 000 to 255:
set /P _green1= Enter GREEN value between 000 to 255:
set /P _blue1= Enter BLUE value between 000 to 255:
echo.
echo Colour 2
set /P _red2= Enter the RED value between 000 to 255:
set /P _green2= Enter GREEN value between 000 to 255:
set /P _blue2= Enter BLUE value between 000 to 255:
echo.
echo Colour 3
set /P _red3= Enter the RED value between 000 to 255:
set /P _green3= Enter GREEN value between 000 to 255:
set /P _blue3= Enter BLUE value between 000 to 255:
echo static %_red1% %_green1% %_blue1% %_red2% %_green2% %_blue2% %_red3% %_green3% %_blue3% > %comp%
goto start

:supercar
echo.
set /P _red= Enter the RED value between 000 to 255:
set /P _green= Enter GREEN value between 000 to 255:
set /P _blue= Enter BLUE value between 000 to 255:
echo supercar %_red% %_green% %_blue% > %comp%
goto start

:flux
echo.
set /P _red= Enter the RED value between 000 to 255:
set /P _green= Enter GREEN value between 000 to 255:
set /P _blue= Enter BLUE value between 000 to 255:
echo flux %_red% %_green% %_blue% > %comp%
goto start

:fade
echo.
set /P _numcolour= Enter the number of colours 1 to 3:
if "%_numcolour"=="1" goto :fade1
if "%_numcolour"=="2" goto :fade2
if "%_numcolour"=="3" goto :fade3
goto start

:fade1
echo.
set /P _red= Enter the RED value between 000 to 255:
set /P _green= Enter GREEN value between 000 to 255:
set /P _blue= Enter BLUE value between 000 to 255:
echo fade %_red% %_green% %_blue% > %comp%
goto start

:fade2
echo.
echo Colour 1
set /P _red1= Enter the RED value between 000 to 255:
set /P _green1= Enter GREEN value between 000 to 255:
set /P _blue1= Enter BLUE value between 000 to 255:
echo.
echo Colour 2
set /P _red2= Enter the RED value between 000 to 255:
set /P _green2= Enter GREEN value between 000 to 255:
set /P _blue2= Enter BLUE value between 000 to 255:
echo fade %_red1% %_green1% %_blue1% %_red2% %_green2% %_blue2% > %comp%
goto start

:fade3
echo.
echo Colour 1
set /P _red1= Enter the RED value between 000 to 255:
set /P _green1= Enter GREEN value between 000 to 255:
set /P _blue1= Enter BLUE value between 000 to 255:
echo.
echo Colour 2
set /P _red2= Enter the RED value between 000 to 255:
set /P _green2= Enter GREEN value between 000 to 255:
set /P _blue2= Enter BLUE value between 000 to 255:
echo.
echo Colour 3
set /P _red3= Enter the RED value between 000 to 255:
set /P _green3= Enter GREEN value between 000 to 255:
set /P _blue3= Enter BLUE value between 000 to 255:
echo fade %_red1% %_green1% %_blue1% %_red2% %_green2% %_blue2% %_red3% %_green3% %_blue3% > %comp%
goto start

:wipe
echo.
set /P _numcolour= Enter the number of colours 1 to 3:
if "%_numcolour"=="1" goto :wipe1
if "%_numcolour"=="2" goto :wipe2
if "%_numcolour"=="3" goto :wipe3
goto start

:wipe1
echo.
set /P _red= Enter the RED value between 000 to 255:
set /P _green= Enter GREEN value between 000 to 255:
set /P _blue= Enter BLUE value between 000 to 255:
echo wipe %_red% %_green% %_blue% > %comp%
goto start

:wipe2
echo.
echo Colour 1
set /P _red1= Enter the RED value between 000 to 255:
set /P _green1= Enter GREEN value between 000 to 255:
set /P _blue1= Enter BLUE value between 000 to 255:
echo.
echo Colour 2
set /P _red2= Enter the RED value between 000 to 255:
set /P _green2= Enter GREEN value between 000 to 255:
set /P _blue2= Enter BLUE value between 000 to 255:
echo wipe %_red1% %_green1% %_blue1% %_red2% %_green2% %_blue2% > %comp%
goto start

:wipe3
echo.
echo Colour 1
set /P _red1= Enter the RED value between 000 to 255:
set /P _green1= Enter GREEN value between 000 to 255:
set /P _blue1= Enter BLUE value between 000 to 255:
echo.
echo Colour 2
set /P _red2= Enter the RED value between 000 to 255:
set /P _green2= Enter GREEN value between 000 to 255:
set /P _blue2= Enter BLUE value between 000 to 255:
echo.
echo Colour 3
set /P _red3= Enter the RED value between 000 to 255:
set /P _green3= Enter GREEN value between 000 to 255:
set /P _blue3= Enter BLUE value between 000 to 255:
echo wipe %_red1% %_green1% %_blue1% %_red2% %_green2% %_blue2% %_red3% %_green3% %_blue3% > %comp%
goto start


:running
echo.
set /P _red3= Enter the RED value between 000 to 255:
set /P _green3= Enter GREEN value between 000 to 255:
set /P _blue3= Enter BLUE value between 000 to 255:
echo running %_red% %_green% %_blue% > %comp%
goto start

:meteor
echo.
set /P _red3= Enter the RED value between 000 to 255:
set /P _green3= Enter GREEN value between 000 to 255:
set /P _blue3= Enter BLUE value between 000 to 255:
echo meteor %_red% %_green% %_blue% > %comp%
goto start

:usage
echo RGBling. v.1
echo Usage: RGBling /dev/{arduino_port}
echo e.g. RGBling /dev/ttyUSB0
echo. 
echo RGBling is a set commandline utilites for controlling
echo WS2812B Addressable RGB LED strips connected to Arduino
echo micro controllers using the NEOPixel library.
echo.
goto end

:end
cls
