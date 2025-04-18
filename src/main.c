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
int brush_size = 0;

int place = 0;
int place_type = 1;

int N;
obj *particles = NULL;
obj *particles_next = NULL;
int *particles_moving = NULL;

SDL_Rect **rects = NULL;

void generate() {
	for (int i = 0; i < N * N; i++) {
		particles[i].type = 0;
	}

	// for (int i = 0; i < N; i++) {
	// 	particles[N * N - i - 1].type = 2;
	// }

	memcpy(particles_next, particles, N * N * sizeof(obj));
}

void init(char *args[]) {
	N = atoi(args[1]);
	window_x = pix * N;
	window_y = pix * N;

	srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(window_x, window_y + 50, SDL_WINDOW_SHOWN, &window, &renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 100, 100);

	// TODO: texture color change not currently working
	// SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
	// SDL_SetTextureColorMod(texture, 255, 255, 255);
	// SDL_SetTextureAlphaMod(texture, 255);

	particles = calloc(N * N, sizeof(obj));
	particles_next = calloc(N * N, sizeof(obj));
	particles_moving = calloc(N * N, sizeof(int));

	rects = calloc(9, sizeof(SDL_Rect *));
	for (int i = 0; i < 9; i++) {
		rects[i] = calloc(N * N, sizeof(SDL_Rect));
	}

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

	for (int i = 0; i < 9; i++) {
		free(rects[i]);
	}

	free(rects);
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

			if (y_pos < N - brush_size && y_pos >= brush_size && x_pos < N - brush_size && x_pos >= brush_size) {
				for (int y = y_pos - brush_size; y < y_pos + brush_size + 1; y++) {
					for (int x = x_pos - brush_size; x < x_pos + brush_size + 1; x++) {
						particles[y * N + x].type = place_type;
					}
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
				case SDLK_PLUS:
				case SDLK_EQUALS:
					brush_size++;
					break;
				case SDLK_MINUS:
					brush_size--;
					if (brush_size < 0) brush_size = 0;
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

		if (counter++ == 1000) {
			counter = 0;
			printf("frame: %fms (%f fps)\n", 1000.0 * frame_time, 1 / frame_time);
		}
		// SDL_Delay(75);
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