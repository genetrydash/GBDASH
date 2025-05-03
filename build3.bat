c:\gbdk\bin\lcc -Iinclude -c -o title_theme.o src/title_theme.c
c:\gbdk\bin\lcc.exe -Iinclude -Wl-lhugedriver\gbdk/hUGEDriver.lib -o MyGame.gb src/main.c src/music.c title_theme.o
pause