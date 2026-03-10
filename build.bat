@echo off
REM GBDK-2020 Build Script for Windows
REM 使用前请修改 GBDK_HOME 为你的 GBDK 安装路径
REM 例如：set GBDK_HOME=C:\gbdk

set GBDK_HOME=C:\gbdk
set LCC=%GBDK_HOME%\bin\lcc

set PROJECTNAME=2048
set SRCDIR=src
set OBJDIR=obj

echo Building %PROJECTNAME%.gb...

@if not exist "%OBJDIR%" mkdir "%OBJDIR%"

%LCC% -mgbz80:gb -c -o %OBJDIR%\main.o %SRCDIR%\main.c
%LCC% -mgbz80:gb -c -o %OBJDIR%\game.o %SRCDIR%\game.c

%LCC% -Wl-yt0x1B -Wl-j -Wm-yoA -Wm-ya4 -autobank -Wb-ext=.rel -Wb-v -mgbz80:gb -o %PROJECTNAME%.gb %OBJDIR%\main.o %OBJDIR%\game.o

@if exist %PROJECTNAME%.gb (
    echo.
    echo Build successful!
    echo Run with: SameBoy.exe %PROJECTNAME%.gb
) else (
    echo.
    echo Build failed!
)

pause
