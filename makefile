all: abbaye

abbaye: ./src/main.c ./src/loading.c ./src/startscreen.c ./src/history.c ./src/game.c ./src/jean.c ./src/enemies.c ./src/gameover.c ./src/ending.c ./src/drawing.c
	gcc -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize ./src/main.c ./src/loading.c ./src/startscreen.c ./src/history.c ./src/game.c ./src/jean.c ./src/enemies.c ./src/gameover.c ./src/ending.c ./src/drawing.c -o abbayev2 `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lm

clean:

	rm -rf ./abbayev2

# Installation
install:
	cp abbayev2 /usr/bin/
	cp abbaye.desktop /usr/share/applications
	mkdir -p /usr/share/abbayev2/sounds
	cp ./sounds/* /usr/share/abbayev2/sounds
	mkdir -p /usr/share/abbayev2/data
	cp ./data/* /usr/share/abbayev2/data
	cp abbaye.png /usr/share/pixmaps
	mkdir -p /usr/share/abbayev2/graphics
	cp -r ./graphics/* /usr/share/abbayev2/graphics

uninstall:
	rm /usr/bin/abbayev2
	rm /usr/share/applications/abbaye.desktop
	rm /usr/share/pixmaps/abbaye.png
	rm -rf /usr/share/abbayev2