#include "sim.h"
#include "main.h"

#define p(x, y) (particles[(y) * N + (x)])
#define pn(x, y) (particles_next[(y) * N + (x)])

#define check(p, PROP) ((materials[p.type] & PROP) == PROP)
#define density(p1, p2) ((materials[p1.type] & DENSITY) > (materials[p2.type] & DENSITY))

int materials[] = {
	PROP_NONE | PROP_FLOW | 0b00,			  // air
	PROP_FALL | PROP_SINK | PROP_NORM | 0b11, // sand
	PROP_FALL | PROP_FLOW | PROP_NORM | 0b10, // water
	0,										  // rock
	PROP_FALL | PROP_SINK | PROP_SOFT | 0b11, // light sand
	PROP_FALL | PROP_SINK | PROP_HARD | 0b11, // heavy sand
	PROP_FALL | PROP_FLOW | PROP_NORM | 0b11, // dense liquid
	PROP_FALL | PROP_SINK | PROP_NORM | 0b01, // cork
	0,
	0,
	0};

void update_pixel(int dest_x, int dest_y, int src_x, int src_y) {
	int tmp_type = pn(dest_x, dest_y).type;

	pn(dest_x, dest_y).type = pn(src_x, src_y).type;
	pn(src_x, src_y).type = tmp_type;

	particles_moving[dest_y * N + dest_x] = 1;
}

void update_sand() {
	memset(particles_moving, 0, N * N * sizeof(int));

	int dir = 2 * rng(1) - 1; // randomly -1 or +1

	// NOTE: this loop does not run for materials on the screen floor
	for (int y = N - 2; y >= 0; y--) {
		for (int x_ind = 0; x_ind < N; x_ind++) {
			// sand
			int x = x_ind;
			if (dir == 1) x = N - x_ind - 1;

			if (pn(x, y).type == 0) continue; // ignore air

			// falling materials move through flowing materials
			if (check(pn(x, y), PROP_FALL)) {
				if (check(pn(x, y + 1), PROP_NONE)) {
					update_pixel(x, y + 1, x, y);
				}
			}

			if (pn(x, y).type == 0) continue; // ignore air

			if (check(pn(x, y), PROP_SINK)) {
				if (pn(x, y).type != pn(x, y + 1).type && check(pn(x, y + 1), PROP_FLOW) && density(pn(x, y), pn(x, y + 1))) {
					update_pixel(x, y + 1, x, y);
				}
			}

			if (pn(x, y).type == 0) continue; // ignore air

			// materials with normal weight spread into flowing materials
			if (check(pn(x, y), PROP_NORM)) {
				if (x < N - 1 && check(pn(x + 1, y + 1), PROP_FLOW) && density(pn(x, y), pn(x + 1, y + 1))) {
					update_pixel(x + 1, y + 1, x, y);
				} else if (x > 0 && check(pn(x - 1, y + 1), PROP_FLOW) && density(pn(x, y), pn(x - 1, y + 1))) {
					update_pixel(x - 1, y + 1, x, y);
				}
			}

			if (pn(x, y).type == 0) continue; // ignore air

			// soft materials spread further to the sides
			if (check(pn(x, y), PROP_SOFT)) {
				for (int i = 2; i < 4; i++) {
					if (x < N - i && check(pn(x + i, y + 1), PROP_FLOW)) {
						update_pixel(x + i, y + 1, x, y);
					} else if (x >= i && check(pn(x - i, y + 1), PROP_FLOW)) {
						update_pixel(x - i, y + 1, x, y);
					}
				}
			}

			if (pn(x, y).type == 0) continue; // ignore air

			// hard materials spread less
			if (check(pn(x, y), PROP_HARD)) {
				if (y < N - 4) {
					if (x < N - 1 && check(pn(x + 1, y + 4), PROP_FLOW)) {
						update_pixel(x + 1, y + 4, x, y);
					} else if (x > 0 && check(pn(x - 1, y + 4), PROP_FLOW)) {
						update_pixel(x - 1, y + 4, x, y);
					}
				}
			}

			if (pn(x, y).type == 0) continue; // ignore air

			// flowing materials spread out sideways
			if (check(pn(x, y), PROP_FLOW)) {
				// TODO: this should not run if current pixel is the top pixel

				if (x + 1 < window_x && x > 0) {
					int type = pn(x, y).type;
					if (type == pn(x + 1, y).type && check(pn(x - 1, y), PROP_FLOW) && density(pn(x, y), pn(x - 1, y))) {
						update_pixel(x - 1, y, x, y);
					} else if (type == pn(x - 1, y).type && check(pn(x + 1, y), PROP_FLOW) && density(pn(x, y), pn(x + 1, y))) {
						update_pixel(x + 1, y, x, y);
					}
				}
			}
			// if (check(pn(x, y), PROP_FLOW)) {
			// 	// TODO: this should not run if current pixel is the top pixel

			// 	int type = pn(x, y).type;
			// 	// attempt to move away if there is a pixel above of the same type
			// 	if (type == pn(x, y - 1).type) {
			// 		for (int dist = 1; x + dist < window_x; dist++) {
			// 			if (check(pn(x + dist, y), PROP_NONE)) {
			// 				// empty space to move into
			// 				update_pixel(x + dist, y, x, y);
			// 				goto end;
			// 			} else if (type != pn(x + dist, y).type) {
			// 				break;
			// 			}
			// 		}
			// 		for (int dist = 1; x - dist >= 0; dist++) {
			// 			if (check(pn(x - dist, y), PROP_NONE)) {
			// 				// empty space to move into
			// 				update_pixel(x - dist, y, x, y);
			// 				goto end;
			// 			} else if (type != pn(x - dist, y).type) {
			// 				break;
			// 			}
			// 		}
			// 	}
			// end:
			// }

			// if (check(pn(x, y), PROP_FLOW)) {
			// 	// flow sideways
			// 	if (x < N - 1 && check(pn(x + 1, y), PROP_NONE)) {
			// 		update_pixel(x + 1, y, x, y);
			// 	} else if (x > 0 && check(pn(x - 1, y), PROP_NONE)) {
			// 		update_pixel(x - 1, y, x, y);
			// 	}
			// }
		}
	}

	// printf("--- changelog ---\n");
	// for (int y = 0; y < N; y++) {
	// 	for (int x = 0; x < N; x++) {
	// 		if (pn(x, y).type != p(x, y).type) {
	// 			printf("(%i, %i) [%i->%i]\n", x, y, p(x, y).type, pn(x, y).type);
	// 		}
	// 		/* code */
	// 	}
	// }

	memcpy(particles, particles_next, N * N * sizeof(obj));
}

void update(float dt) {
	update_sand();
}
