#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "render.h"
#include "sim.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

float PI = 3.1415926535897932384626433832795;

int frame_time = 1000;

int window_x = 500;
int window_y = 500;

int pix = 4;
int place = 0;
int place_type = 1;

int N;
obj *particles = NULL;
obj *particles_next = NULL;
int *particles_moving = NULL;

void sout(char *str) {
	printf("%s\n", str);
}

void generate() {
	for (int i = 0; i < N * N; i++) {
		particles[i].type = 0;
	}

	int size = 20;
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			particles[y * N + x + 2].type = 1;
		}
	}

	memcpy(particles_next, particles, N * N * sizeof(obj));
}

void init(char *args[]) {
	N = atoi(args[1]);
	window_x = pix * N;
	window_y = pix * N;

	srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(window_x, window_y, SDL_WINDOW_SHOWN, &window, &renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 100, 100);

	// TODO: texture color change not currently working
	// SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
	// SDL_SetTextureColorMod(texture, 255, 255, 255);
	// SDL_SetTextureAlphaMod(texture, 255);

	particles = calloc(sizeof(obj), N * N);
	particles_next = calloc(sizeof(obj), N * N);
	particles_moving = calloc(sizeof(int), N * N);
	generate();
}

void end() {
	printf("quitting\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	free(particles);
	free(particles_next);
	free(particles_moving);
}

void run() {
	printf("\nrunning :)\n");

	SDL_Event event;

	int counter = 0;

	Uint64 prev_time = SDL_GetPerformanceCounter();

	vec2 mouse_pos;
	mouse_pos.x = 0;
	mouse_pos.y = 0;

	int running = 1;
	while (running) {
		Uint64 frame_start = SDL_GetPerformanceCounter();

		if (place == 1) {
			int x_pos = mouse_pos.x;
			int y_pos = mouse_pos.y;

			particles[(y_pos)*N + x_pos].type = place_type;
			if (y_pos < N - 1) {
				particles[(1 + y_pos) * N + x_pos].type = place_type;
			}
			if (x_pos < N - 1) {
				particles[(y_pos)*N + 1 + x_pos].type = place_type;
				if (y_pos < N - 1) {
					particles[(1 + y_pos) * N + 1 + x_pos].type = place_type;
				}
			}
			memcpy(particles_next, particles, N * N * sizeof(obj));
		}

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				running = 0;
				break;
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_q:
					running = 0;
					break;
				case SDLK_r:
					generate();
					break;
				case SDLK_0:
					place_type = 0;
					break;
				case SDLK_1:
					place_type = 1;
					break;
				case SDLK_2:
					place_type = 2;
					break;
				case SDLK_3:
					place_type = 3;
					break;
				case SDLK_4:
					place_type = 4;
					break;
				case SDLK_5:
					place_type = 5;
					break;
				case SDLK_6:
					place_type = 6;
					break;
				case SDLK_7:
					place_type = 7;
					break;
				case SDLK_8:
					place_type = 8;
					break;
				case SDLK_9:
					place_type = 9;
					break;
				default:
					break;
				}
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				place = 1;
			} else if (event.type == SDL_MOUSEBUTTONUP) {
				place = 0;
			} else if (event.type == SDL_MOUSEMOTION) {
				if (event.button.x >= 0 && event.button.y >= 0 && event.button.x < window_x && event.button.y < window_y) {
					mouse_pos.x = event.button.x / pix;
					mouse_pos.y = event.button.y / pix;
				}
			}
		}

		Uint64 curr_time = SDL_GetPerformanceCounter();
		float dt = (curr_time - prev_time) / (float)SDL_GetPerformanceFrequency();
		prev_time = curr_time;
		render();
		update(dt);

		Uint64 frame_end = SDL_GetPerformanceCounter();
		float frame_time = (frame_end - frame_start) / (float)SDL_GetPerformanceFrequency();

		if (counter++ == 5000) {
			counter = 0;
			printf("frame: %fms (%f fps)\n", 1000.0 * frame_time, 1 / frame_time);
		}
		SDL_Delay(2);
	}
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		init(argv);
		run();
		end();

		return EXIT_SUCCESS;
	}

	printf("specify sim size\n");
	return EXIT_FAILURE;
}