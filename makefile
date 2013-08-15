# Compilation

all: abbaye abbaye-pandora

abbaye: ./src/main.c ./src/barradeestado.c ./src/bosses.c ./src/enemigos.c ./src/fase.c ./src/jean.c ./src/musica.c ./src/pantallas.c ./src/comun.h
	gcc -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize ./src/main.c ./src/barradeestado.c ./src/bosses.c ./src/enemigos.c ./src/fase.c ./src/jean.c ./src/musica.c ./src/pantallas.c -o abbaye `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lm

abbaye-pandora: ./src/main.c ./src/barradeestado.c ./src/bosses.c ./src/enemigos.c ./src/fase.c ./src/jean.c ./src/musica.c ./src/pantallas.c ./src/comun.h
	$(PREFIX)gcc -O3 -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize ./src/main.c ./src/barradeestado.c ./src/bosses.c ./src/enemigos.c ./src/fase.c ./src/jean.c ./src/musica.c ./src/pantallas.c -o abbaye-pandora `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lm -D_OPENPANDORA

clean:
	rm -f abbaye
	rm -f abbaye-pandora

# Installation
install:
	cp abbaye /usr/bin/
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
	rm /usr/bin/abbaye
	rm /usr/share/applications/abbaye.desktop
	rm /usr/share/pixmaps/abbaye.png
	rm -rf /usr/share/abbaye
