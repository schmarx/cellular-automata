#include "sim.h"
#include "main.h"

#define p(x, y) (particles[(y) * N + (x)])
#define pn(x, y) (particles_next[(y) * N + (x)])

void update_sand() {

	// TODO: currently there is a bias to the direction considered first, might be best to use a random choice when the outcome is not determinate
	for (int y = N - 1; y >= 0; y--) {
		for (int x = 0; x < N; x++) {
			// sand
			if (pn(x, y).type == 1 || pn(x, y).type == 4) {
				if (y < N - 1) {
					if (pn(x, y + 1).type == 0) {
						pn(x, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (x < N - 1 && pn(x + 1, y + 1).type == 0) {
						pn(x + 1, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (x > 0 && pn(x - 1, y + 1).type == 0) {
						pn(x - 1, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					}
				}
			}
			if (pn(x, y).type == 4) {
				if (y < N - 1) {
					if (x < N - 2 && pn(x + 2, y + 1).type == 0) {
						pn(x + 2, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (x > 1 && pn(x - 2, y + 1).type == 0) {
						pn(x - 2, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (x < N - 3 && pn(x + 3, y + 1).type == 0) {
						pn(x + 3, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (x > 2 && pn(x - 3, y + 1).type == 0) {
						pn(x - 3, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					}
				}
			}

			if (pn(x, y).type == 5) {
				if (y < N - 1) {
					if (pn(x, y + 1).type == 0) {
						pn(x, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (y < N - 4 && x < N - 1 && pn(x + 1, y + 4).type == 0) {
						pn(x + 1, y + 4).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (y < N - 4 && x > 0 && pn(x - 1, y + 4).type == 0) {
						pn(x - 1, y + 4).type = pn(x, y).type;
						pn(x, y).type = 0;
					}
				}
			}

			// water
			if (pn(x, y).type == 2) {
				if (y < N - 1) {
					if (pn(x, y + 1).type == 0) {
						pn(x, y + 1).type = pn(x, y).type;
						pn(x, y).type = 0;
					} else if (x < N - 1) {
						if (pn(x, y - 1).type == 2) {
							if (pn(x + 1, y).type == 0) {
								pn(x + 1, y).type = pn(x, y).type;
								pn(x, y).type = 0;
								printf("shift\n");
							} else if (x > 0) {
								if (pn(x - 1, y).type == 0) {
									pn(x - 1, y).type = pn(x, y).type;
									pn(x, y).type = 0;
								}
							}
						}
					}
				}
			}
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
