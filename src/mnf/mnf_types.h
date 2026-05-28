#ifndef MANIFOLD_TYPES_H
#define MANIFOLD_TYPES_H

enum MNF_COORD {
    X = 0,
    Y,
    Z,
    W,
};


typedef int     ivec2[2];
typedef int     ivec3[3];

typedef float   vec2[2];
typedef float   vec3[3];
typedef float   vec4[4];
//typedef __attribute__((aligned(16))) float vec4[4];
//typedef __attribute__((aligned(16))) vec4 mat4[4];

typedef vec3    mat3[3];             /* column-major [col][row] */
typedef vec4    mat4[4];

#endif /* MANIFOLD_TYPES_H */
