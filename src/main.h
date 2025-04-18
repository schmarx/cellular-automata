#include <SDL2/SDL.h>
#define rng(max) (rand() % (max + 1))

#define PROP_NONE 0b00000001
#define PROP_FALL 0b00000010
#define PROP_FLOW 0b00000100
#define PROP_SINK 0b00001000
#define PROP_SOFT 0b00010000
#define PROP_NORM 0b00100000
#define PROP_HARD 0b01000000

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