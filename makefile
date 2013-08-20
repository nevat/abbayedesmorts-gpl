all: abbaye

abbaye: ./src/main.c ./src/loading.c ./src/startscreen.c ./src/history.c ./src/game.c ./src/jean.c ./src/enemies.c ./src/gameover.c ./src/ending.c ./src/drawing.c
	gcc -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize ./src/main.c ./src/loading.c ./src/startscreen.c ./src/history.c ./src/game.c ./src/jean.c ./src/enemies.c ./src/gameover.c ./src/ending.c ./src/drawing.c -o ./src/abbaye `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

clean:

	rm -rf ./src/abbaye
