#include <SDL2/SDL.h>
#define rng(max) (rand() % (max + 1))

extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

extern int window_x;
extern int window_y;

extern float PI;

typedef struct {
	float x;
	float y;
} vec2;

typedef struct {
	vec2 pos;
	int type;
} obj;

extern obj *particles;
extern obj *particles_next;
// extern int *particle_counts;
extern int N;