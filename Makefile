run:
	gcc -O3 ./src/main.c ./src/sim.c ./src/render.c -o ./bin/main.o -L/usr/include/SDL2/ -lSDL2
	./bin/main.o ${N}