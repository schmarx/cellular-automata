#include "render.h"
#include "main.h"

float d = 1;

void render() {
	// clear
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderClear(renderer);

	// add shapes

	// TODO: look into rendering multiple objects at once (...FillRects instead of ...FillRect for example)

	d = window_x / N;
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {

			SDL_Rect rect;
			rect.h = d;
			rect.w = d;

			rect.x = x * window_x / N;
			rect.y = y * window_y / N;

			if (particles[y * N + x].type == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}

			// printf("(%i, %i)\n", rect.x, rect.y);
			// SDL_RenderCopyEx(renderer, texture, NULL, &rect, particles[i].rot, 0, SDL_FLIP_NONE);
		}
	}

	// draw to screen
	SDL_RenderPresent(renderer);
}