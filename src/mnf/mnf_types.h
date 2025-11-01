/* Manifold: Graphics Math Library for Sage.

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

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
