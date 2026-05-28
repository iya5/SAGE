#ifndef MANIFOLD_UTIL_H
#define MANIFOLD_UTIL_H

#define PI 3.14159f
#define MNF_RAD(DEG) ((DEG) * (PI / 180.0f))
#define MNF_DEG(RAD) ((RAD) * (180.0f / PI))

#define WOLRD_UP (vec3){0.0f, 1.0f, 0.0f}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAMP(value, min, max) ((min < max) \
	? (value < min) ? min : ((value > max) ? max : value) \
	: (value < max) ? max : ((value > min) ? min : value))

#endif /* MANIFOLD_UTIL_H */
