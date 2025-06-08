@echo off
setlocal EnableDelayedExpansion

:: Configuración de directorios
set SRC_DIR=src
set MUSIC_DIR=%SRC_DIR%\music
set INC_DIR=include
set BUILD_DIR=build
set BUILD_MUSIC_DIR=%BUILD_DIR%\music

:: Herramientas y flags
set CC=gbdk\bin\lcc
set CFLAGS=-I%INC_DIR% -I%SRC_DIR% -c -debug
set ROM_TITLE=GB_DASH
set LDFLAGS=-I%INC_DIR% -I%SRC_DIR% -Wl-lhugedriver/gbdk/hUGEDriver.lib -Wl-yt19 -Wl-yo8 -debug

:: Nombre del archivo final
set TARGET=%BUILD_DIR%\game.gb

echo Ejecutando processtotxt.py...
python processtotxt.py
if errorlevel 1 (
    echo Error ejecutando processtotxt.py
    exit /b 1
)

:: Crear directorios si no existen
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
if not exist %BUILD_MUSIC_DIR% mkdir %BUILD_MUSIC_DIR%

:: Compilar archivos fuente
echo Compilando archivos fuente...

for %%f in (%SRC_DIR%\*.c) do (
    set FILE=%%~nxf
    echo Compilando %%f
    %CC% %CFLAGS% -o %BUILD_DIR%\%%~nf.o %%f
    if errorlevel 1 exit /b 1
)

for %%f in (%MUSIC_DIR%\*.c) do (
    set FILE=%%~nxf
    echo Compilando %%f
    %CC% %CFLAGS% -o %BUILD_MUSIC_DIR%\%%~nf.o %%f
    if errorlevel 1 exit /b 1
)

:: Recolectar todos los .o para enlazado
set OBJECTS=
for %%f in (%BUILD_DIR%\*.o) do (
    set OBJECTS=!OBJECTS! %%f
)
for %%f in (%BUILD_MUSIC_DIR%\*.o) do (
    set OBJECTS=!OBJECTS! %%f
)

:: Enlazar archivo final
echo Enlazando %TARGET%...
%CC% %LDFLAGS% -o %TARGET% %OBJECTS%
if errorlevel 1 (
    echo Error al enlazar
    exit /b 1
)

echo Compilación completada exitosamente.
pause