#
# use `make PLATFORM`, linux build is default
#

NAME = abbaye
SRC = ./src/main.c \
      ./src/barradeestado.c \
      ./src/bosses.c \
      ./src/enemigos.c \
      ./src/fase.c \
      ./src/jean.c \
      ./src/musica.c \
      ./src/pantallas.c

# Some platforms do not have (useable) sdl-config
ifeq ($(NO_SDL_CONFIG),)
    SDL_CFLAGS := `sdl-config --cflags`
    SDL_LIBS   := `sdl-config --libs`
endif

CFLAGS = -s $(SDL_CFLAGS)
LIBS   = $(SDL_LIBS) -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lm

# Cross-Compilation
ifeq ($(PLATFORM),gcw0)
    CC     = mipsel-linux-gcc
    CFLAGS += -mips32 -D_GCW_ZERO
else ifeq ($(PLATFORM),pandora)
    NAME   = abbaye-pandora
    CC     = $(PREFIX)gcc
    CFLAGS += -D_OPENPANDORA -finline-functions -funswitch-loops \
              -fpredictive-commoning -fgcse-after-reload -ftree-vectorize
else ifeq ($(PLATFORM),wii)
    NAME   = abbaye-wii.elf
    CC     = powerpc-eabi-gcc
    CFLAGS += -O2 -D_WII -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float \
              -I$(DEVKITPRO)/libogc/include -I$(DEVKITPRO)/libogc/include/SDL
    LIBS   := -L$(DEVKITPRO)/libogc/lib/wii -lSDL_image -lSDL_ttf -lSDL_mixer \
              -lSDL_gfx -lSDL -lfat -logc -lwiikeyboard -lwiiuse -lbte \
              -L$(DEVKITPRO)/portlibs/ppc/lib -lpng -lz -ljpeg -lfreetype \
              -lmad -lvorbisidec -lm
else
    CC = gcc

    # detect mingw and do some fixes
    ifeq ($(OS),Windows_NT)
        CFLAGS  += -Dsleep=SDL_Delay -D_RUTAS_RELATIVAS
        NAME    = abbaye.exe
    endif
endif

# platform magic
ifeq ($(PLATFORM),)
all: linux
else ifeq ($(PLATFORM),gcw0)
all: opk
# this is currently disabled, as there is no script to build the .pnd:
#else ifeq ($(PLATFORM),"pandora")
#all: pnd
else ifeq ($(PLATFORM),wii)
all: dol
else
all: $(NAME)
endif

# Common
.PHONY: clean
clean:
	rm -f abbaye abbaye.exe abbaye-pandora abbaye.pnd \
	      abbaye.opk abbaye-wii.elf boot.dol
	@echo "Cleaned all built files."

$(NAME): $(SRC) ./src/comun.h
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) $(LIBS) -o $(NAME)

# Linux
linux: $(NAME)
	@echo "Linux build ready."

# Pandora
pandora:
	$(MAKE) PLATFORM=pandora
	@echo "Pandora build ready."

pnd: $(NAME)
	@echo "build script for abbaye.pnd missing :("

# GCW Zero
gcw0:
	$(MAKE) PLATFORM=gcw0
	@echo "GCW-Zero build ready."

opk: $(NAME)
	./make_opk.sh

# Wii
wii:
	$(MAKE) PLATFORM=wii NO_SDL_CONFIG=1
	@echo "Wii build ready."

dol: $(NAME)
	elf2dol abbaye-wii.elf boot.dol

# Installation under Linux
install: linux
	@echo "Installing linux build..."
	mkdir -p $(DESTDIR)/usr/bin
	cp $(NAME) $(DESTDIR)/usr/bin
	mkdir -p $(DESTDIR)/usr/share/applications
	cp abbaye.desktop $(DESTDIR)/usr/share/applications
	mkdir -p $(DESTDIR)/usr/share/abbaye
	cp -r ./fonts ./graphics ./music ./sounds $(DESTDIR)/usr/share/abbaye
	mkdir -p $(DESTDIR)/usr/share/abbaye/data
	cp ./mapa/* $(DESTDIR)/usr/share/abbaye/data
	mkdir -p $(DESTDIR)/usr/share/pixmaps
	cp abbaye.png $(DESTDIR)/usr/share/pixmaps

uninstall:
	rm /usr/bin/$(NAME)
	rm /usr/share/applications/abbaye.desktop
	rm /usr/share/pixmaps/abbaye.png
	rm -rf /usr/share/abbaye
