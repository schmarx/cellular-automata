#include "sim.h"
#include "main.h"

#define p(x, y) (particles[(y) * N + (x)])
#define pn(x, y) (particles_next[(y) * N + (x)])

#define check(p, PROP) ((materials[p.type] & PROP) == PROP)

int materials[] = {
	PROP_NONE | PROP_FLOW,			   // air
	PROP_FALL | PROP_SINK | PROP_NORM, // sand
	PROP_FALL | PROP_FLOW,			   // water
	0,								   // rock
	PROP_FALL | PROP_SINK | PROP_SOFT, // light sand
	PROP_FALL | PROP_SINK | PROP_HARD, // heavy sand
};

void update_pixel(int dest_x, int dest_y, int src_x, int src_y) {
	int tmp_type = pn(dest_x, dest_y).type;
	int tmp_vel = pn(dest_x, dest_y).vel;

	pn(dest_x, dest_y).type = pn(src_x, src_y).type;
	pn(dest_x, dest_y).vel = pn(src_x, src_y).vel;

	pn(src_x, src_y).type = tmp_type;
	pn(src_x, src_y).vel = tmp_vel;

	particles_moving[dest_y * N + dest_x] = 1;
}

void update_sand() {
	memset(particles_moving, 0, N * N * sizeof(int));

	int val = 2 * rng(1) - 1; // randomly -1 or +1

	// NOTE: this loop does not run for materials on the screen floor
	for (int y = N - 2; y >= 0; y--) {
		for (int x_ind = 0; x_ind < N; x_ind++) {
			// sand
			int x = x_ind;
			if (val == 1) x = N - x_ind - 1;

			if (pn(x, y).type == 0) continue; // ignore air

			// falling materials move through flowing materials
			if (check(pn(x, y), PROP_FALL)) {
				if (check(pn(x, y + 1), PROP_FLOW) && pn(x, y).type != pn(x, y + 1).type) {
					update_pixel(x, y + 1, x, y);
				}
			}

			// materials with normal weight spread into flowing materials
			if (check(pn(x, y), PROP_NORM)) {
				if (x < N - 1 && check(pn(x + 1, y + 1), PROP_FLOW)) {
					update_pixel(x + 1, y + 1, x, y);
				} else if (x > 0 && check(pn(x - 1, y + 1), PROP_FLOW)) {
					update_pixel(x - 1, y + 1, x, y);
				}
			}

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

			// flowing materials spread out sideways
			if (check(pn(x, y), PROP_FLOW)) {
				// this checks the directions the material is able to move
				int allow = 0;
				if (x < N - 1 && check(pn(x + 1, y), PROP_NONE)) {
					allow += 1;
				}
				if (x > 0 && check(pn(x - 1, y), PROP_NONE)) {
					allow += 2;
				}

				// this sets the velocity depending on the allowed direction
				if (allow == 0) {
					// if no direction is allowed remove velocity
					pn(x, y).vel = 0;
					continue;

				} else if (allow == 1) {
					// only moving right allowed
					pn(x, y).vel = 1;
				} else if (allow == 2) {
					// only moving left allowed
					pn(x, y).vel = -1;
				} else if (allow == 3) {
					// both allowed
					// if a direction is already set we will keep that direction, else randomly choose a direction
					if (pn(x, y).vel == 0) {
						pn(x, y).vel = 2 * rng(1) - 1; // randomly -1 or +1
					} else {
						pn(x, y).vel *= 2;
					}
				}

				int vel = 0;
				if (pn(x, y).vel > 0) vel = 1;
				if (pn(x, y).vel < 0) vel = -1;
				// update pixel based on the chosen velocity (may be zero)
				update_pixel(x + vel, y, x, y);
			}

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
