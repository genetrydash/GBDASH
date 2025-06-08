@echo off
setlocal EnableDelayedExpansion

:: Configure the directories
set SRC_DIR=src
set MUSIC_DIR=%SRC_DIR%\music
set INC_DIR=include
set BUILD_DIR=build
set BUILD_MUSIC_DIR=%BUILD_DIR%\music

:: Tools and flags
set CC=gbdk\bin\lcc
set CFLAGS=-I%INC_DIR% -I%SRC_DIR% -c -debug
set ROM_TITLE=GB_DASH
set LDFLAGS=-I%INC_DIR% -I%SRC_DIR% -Wl-lhugedriver/gbdk/hUGEDriver.lib -Wl-yt19 -Wl-yo8 -debug

:: Output File
set TARGET=%BUILD_DIR%\game.gb

echo Running processtotxt.py...
python processtotxt.py
if errorlevel 1 (
    echo Error Running processtotxt.py
    exit /b 1
)

:: Create directories if they don't exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
if not exist %BUILD_MUSIC_DIR% mkdir %BUILD_MUSIC_DIR%

:: Compile Source Files
echo Compiling Source Files...

for %%f in (%SRC_DIR%\*.c) do (
    set FILE=%%~nxf
    cls
    echo Compiling %%f
    %CC% %CFLAGS% -o %BUILD_DIR%\%%~nf.o %%f
    if errorlevel 1 exit /b 1
)

for %%f in (%MUSIC_DIR%\*.c) do (
    set FILE=%%~nxf
    cls
    echo Compiling %%f
    %CC% %CFLAGS% -o %BUILD_MUSIC_DIR%\%%~nf.o %%f
    if errorlevel 1 exit /b 1
)

:: Collect all .o files for linking
set OBJECTS=
for %%f in (%BUILD_DIR%\*.o) do (
    set OBJECTS=!OBJECTS! %%f
)
for %%f in (%BUILD_MUSIC_DIR%\*.o) do (
    set OBJECTS=!OBJECTS! %%f
)

:: Link The Final File
cls
echo Linking %TARGET%...
%CC% %LDFLAGS% -o %TARGET% %OBJECTS%
if errorlevel 1 (
    echo Error Linking
    exit /b 1
)
cls
echo Compilation Completed Sucessfully.
pause