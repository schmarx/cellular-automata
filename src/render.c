#include "render.h"
#include "main.h"

float d = 1;

void render() {
	// clear
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// background
	SDL_Rect bg;
	bg.w = window_x;
	bg.h = window_y;
	bg.x = 0;
	bg.y = 0;

	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, &bg);

	int rect_counts[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	d = window_x / N;

	// add shapes
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < N; x++) {
			int type = particles[y * N + x].type;

			if (type != 0) {

				rects[type - 1][rect_counts[type - 1]].h = d;
				rects[type - 1][rect_counts[type - 1]].w = d;

				rects[type - 1][rect_counts[type - 1]].x = x * window_x / N;
				rects[type - 1][rect_counts[type - 1]].y = y * window_y / N;

				rect_counts[type - 1]++;
			}
		}
	}

	// render groups of rectangles
	for (int i = 0; i < 9; i++) {
		int type = i + 1;

		// draw outline around selected brush
		if (place_type == type) {
			SDL_Rect rect;
			rect.h = 24;
			rect.w = 24;
			rect.x = (i + 1) * window_x / 20 - 2;
			rect.y = window_y + 20 - 2;

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rect);
		}

		SDL_Rect rect;
		rect.h = 20;
		rect.w = 20;
		rect.x = (i + 1) * window_x / 20;
		rect.y = window_y + 20;

		if (type == 1) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		} else if (type == 2) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		} else if (type == 3) {
			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		} else if (type == 4) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
		} else if (type == 5) {
			SDL_SetRenderDrawColor(renderer, 150, 150, 0, 255);
		} else if (type == 6) {
			SDL_SetRenderDrawColor(renderer, 50, 50, 0, 255);
		} else {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}

		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderFillRects(renderer, rects[i], rect_counts[i]);
	}

	// draw to screen
	SDL_RenderPresent(renderer);
}