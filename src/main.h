#include <SDL2/SDL.h>
#define rng(max) (rand() % (max + 1))

#define DENSITY 0b0000000011
#define PROP_NONE 0b0000000100
#define PROP_FALL 0b0000001000
#define PROP_FLOW 0b0000010000
#define PROP_SINK 0b0000100000
#define PROP_SOFT 0b0001000000
#define PROP_NORM 0b0010000000
#define PROP_HARD 0b0100000000

extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

extern int window_x;
extern int window_y;

extern int place_type;

extern float PI;

typedef struct {
	float x;
	float y;
} vec2;

typedef struct {
	int vel;
	int type;
} obj;

extern obj *particles;
extern obj *particles_next;
extern int *particles_moving;

extern SDL_Rect **rects;

extern int N;