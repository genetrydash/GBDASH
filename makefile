CC=lcc
CFLAGS=-Wa-l -Wl-m
LDFLAGS=-Wl-j

main: main.c
	$(CC) $(CFLAGS) -o main.gb main.c

nightly: main
	lcc -Wa-l -Wl-m -o nightly.gb main.c
	zip -r nightly.zip nightly.gb
	git add nightly.zip
	git commit -m "Nightly build"
	git push origin master
