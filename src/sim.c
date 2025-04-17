#include "sim.h"
#include "main.h"

#define p(x, y) (particles[(y) * N + (x)])
#define pn(x, y) (particles_next[(y) * N + (x)])

void update_pixel(int dest_x, int dest_y, int src_x, int src_y) {
	int tmp_type = pn(dest_x, dest_y).type;
	pn(dest_x, dest_y).type = pn(src_x, src_y).type;
	pn(src_x, src_y).type = tmp_type;

	particles_moving[dest_y * N + dest_x] = 1;
}

void update_sand() {
	memset(particles_moving, 0, N * N * sizeof(int));

	// TODO: currently there is a bias to the direction considered first, might be best to use a random choice when the outcome is not determinate
	for (int y = N - 1; y >= 0; y--) {
		for (int x = 0; x < N; x++) {
			// sand
			if (pn(x, y).type == 1 || pn(x, y).type == 4) {
				if (y < N - 1) {
					if (pn(x, y + 1).type == 0 || pn(x, y + 1).type == 2) {
						update_pixel(x, y + 1, x, y);
					} else if (x < N - 1 && (pn(x + 1, y + 1).type == 0 || pn(x + 1, y + 1).type == 2)) {
						update_pixel(x + 1, y + 1, x, y);
					} else if (x > 0 && (pn(x - 1, y + 1).type == 0 || pn(x - 1, y + 1).type == 2)) {
						update_pixel(x - 1, y + 1, x, y);
					}
				}
			}
			if (pn(x, y).type == 4) {
				if (y < N - 1) {
					if (x < N - 2 && pn(x + 2, y + 1).type == 0) {
						update_pixel(x + 2, y + 1, x, y);
					} else if (x > 1 && pn(x - 2, y + 1).type == 0) {
						update_pixel(x - 2, y + 1, x, y);
					} else if (x < N - 3 && pn(x + 3, y + 1).type == 0) {
						update_pixel(x + 3, y + 1, x, y);
					} else if (x > 2 && pn(x - 3, y + 1).type == 0) {
						update_pixel(x - 3, y + 1, x, y);
					}
				}
			}

			if (pn(x, y).type == 5) {
				if (y < N - 1) {
					if (pn(x, y + 1).type == 0) {
						update_pixel(x, y + 1, x, y);
					} else if (y < N - 4 && x < N - 1 && pn(x + 1, y + 4).type == 0) {
						update_pixel(x + 1, y + 4, x, y);
					} else if (y < N - 4 && x > 0 && pn(x - 1, y + 4).type == 0) {
						update_pixel(x - 1, y + 4, x, y);
					}
				}
			}

			// water
			if (pn(x, y).type == 2) {
				if (y < N - 1) {
					if (pn(x, y + 1).type == 0) {
						update_pixel(x, y + 1, x, y);
					} else if (x < N - 1 && pn(x + 1, y).type == 0) {
						update_pixel(x + 1, y, x, y);
					} else if (x > 0 && pn(x - 1, y).type == 0) {
						update_pixel(x - 1, y, x, y);
					}
				}
			}

			// if (pn(x, y).type == 2) {
			// 	if (y < N - 1) {
			// 		if (pn(x, y + 1).type == 0) {
			// 			update_pixel(x, y + 1, x, y);
			// 		} else if (x < N - 1) {
			// 			if (pn(x, y - 1).type == 2) {
			// 				if (pn(x + 1, y).type == 0) {
			// 					update_pixel(x + 1, y, x, y);
			// 				} else if (x > 0) {
			// 					if (pn(x - 1, y).type == 0) {
			// 						update_pixel(x - 1, y, x, y);
			// 					}
			// 				}
			// 			}
			// 		}
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
