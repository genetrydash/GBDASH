@echo off
echo exporting game...
c:\gbdk\bin\lcc -Iinclude -Wl-lhugedriver\gbdk/hUGEDriver.lib -o MyGame.gb src/main.c src/music.c src/title_theme.o

DEL src\*.asm
DEL src\*.lst
DEL src\*.ihx
DEL src\*.sym
DEL src\*.o

echo ----- GAME BUILDED ----------

pause