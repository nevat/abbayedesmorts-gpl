PREFIX?=	/usr

CC?=		gcc
CFLAGS?=	-O2 -finline-functions -funswitch-loops -fgcse-after-reload -fpredictive-commoning -ftree-vectorize -Wno-unused-result
LDFLAGS?=	-Wl,-z,relro

CFLAGS+=	`sdl2-config --cflags` -DDATADIR="\"$(PREFIX)/share/abbayev2\""
LIBS=		`sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lm

PROG=		abbayev2
SRCS=		src/drawing.c \
		src/ending.c \
		src/enemies.c \
		src/game.c \
		src/gameover.c \
		src/history.c \
		src/jean.c \
		src/loading.c \
		src/main.c \
		src/startscreen.c

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(PROG) $(LIBS)

clean:
	rm -f ./abbayev2

# Installation
install: $(PROG)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROG) $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/applications
	cp abbaye.desktop $(DESTDIR)$(PREFIX)/share/applications
	mkdir -p $(DESTDIR)$(PREFIX)/share/abbayev2/sounds
	cp ./sounds/* $(DESTDIR)$(PREFIX)/share/abbayev2/sounds
	mkdir -p $(DESTDIR)$(PREFIX)/share/abbayev2/data
	cp ./data/* $(DESTDIR)$(PREFIX)/share/abbayev2/data
	mkdir -p $(DESTDIR)$(PREFIX)/share/pixmaps
	cp abbaye.png $(DESTDIR)$(PREFIX)/share/pixmaps
	mkdir -p $(DESTDIR)$(PREFIX)/share/abbayev2/graphics
	cp -r ./graphics/* $(DESTDIR)$(PREFIX)/share/abbayev2/graphics
	mkdir -p $(DESTDIR)$(PREFIX)/share/appdata
	cp ./appdata/abbaye.appdata.xml $(DESTDIR)$(PREFIX)/share/appdata

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(PROG)
	rm $(DESTDIR)$(PREFIX)/share/applications/abbaye.desktop
	rm $(DESTDIR)$(PREFIX)/share/pixmaps/abbaye.png
	rm -rf $(DESTDIR)$(PREFIX)/share/abbayev2
	rm $(DESTDIR)$(PREFIX)/share/appdata/abbaye.appdata.xml
	# ignore if not empty
	-rmdir $(DESTDIR)$(PREFIX)/bin
	-rmdir $(DESTDIR)$(PREFIX)/share/applications
	-rmdir $(DESTDIR)$(PREFIX)/share/pixmaps
