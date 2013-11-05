#
# use `make TARGET=gcw0` to build for GCW-Zero
# and `make` for normal build
#

#TARGET	= gcw0
#TARGET	= pandora

NAME	= abbaye

CFLAGS	= -s -O2 `sdl-config --cflags`
LIBS 	= -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx `sdl-config --libs` -lm

# assume cross-compilation
ifeq "$(TARGET)" "gcw0"
    CC		= mipsel-linux-gcc
    CFLAGS	+= -mips32 -D_GCW_ZERO
else
ifeq "$(TARGET)" "pandora"
    CC		= $(PREFIX)gcc
    CFLAGS	+= -D_OPENPANDORA \
                   -finline-functions -funswitch-loops -fpredictive-commoning \
		   -fgcse-after-reload -ftree-vectorize
    NAME	= abbaye-pandora
else
    CC		= gcc

    # detect mingw and do some fixes
    ifeq ($(OS),Windows_NT)
        CFLAGS	+= -Dsleep=SDL_Delay -D_RUTAS_RELATIVAS
        NAME	= abbaye.exe
    endif
endif
endif

SRC	= ./src/main.c \
	  ./src/barradeestado.c \
	  ./src/bosses.c \
	  ./src/enemigos.c \
	  ./src/fase.c \
	  ./src/jean.c \
	  ./src/musica.c \
	  ./src/pantallas.c

all: $(NAME)

$(NAME): $(SRC) ./src/comun.h
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(NAME)

clean:
	rm -f $(NAME)

# Installation
install:
	cp $(NAME) /usr/bin/
	cp abbaye.desktop /usr/share/applications
	mkdir -p /usr/share/abbaye/music
	cp ./music/* /usr/share/abbaye/music
	mkdir -p /usr/share/abbaye/fonts
	cp ./fonts/* /usr/share/abbaye/fonts
	mkdir -p /usr/share/abbaye/sounds
	cp ./sounds/* /usr/share/abbaye/sounds
	mkdir -p /usr/share/abbaye/data
	cp ./mapa/* /usr/share/abbaye/data
	cp abbaye.png /usr/share/pixmaps
	mkdir -p /usr/share/abbaye/graphics
	cp -r ./graphics/* /usr/share/abbaye/graphics

uninstall:
	rm /usr/bin/$(NAME)
	rm /usr/share/applications/abbaye.desktop
	rm /usr/share/pixmaps/abbaye.png
	rm -rf /usr/share/abbaye

opk:	$(NAME)
	./make_opk.sh