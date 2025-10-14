/* Math header for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __SAGE_MATH_H__
#define __SAGE_MATH_H__

#include <cglm/cglm.h>

#define X 0
#define Y 1
#define Z 2
#define W 3

#define PI 3.14159
#define DEG_TO_RAD(DEG) ((DEG) * (PI / 180.0))
#define RAD_TO_DEG(RAD) ((RAD) * (180.0 / PI))

#define WOLRD_UP (vec3) {0.0, 1.0, 0.0}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y)))
#define CLAMP(value, min, max) ((min < max) \
	? (value < min) ? min : ((value > max) ? max : value) \
	: (value < max) ? max : ((value > min) ? min : value))


/* 
 * Rotates a matrix 'in' by amount theta (in radians) about the x-axis and
 * writes it to 'out'.
 *
 * Parameters:
 *      in      - Input matrix.
 *      theta   - Angle in radians.
 *      out     - Output matrix.
 *
 * Conceptual explanation can be found in the rotation section of the math.c.
 */
void rotate_x(mat4 in, float theta, mat4 out);

/* 
 * Rotates a matrix 'in' by amount theta (in radians) about the y-axis and
 * writes it to 'out'.
 *
 * Parameters:
 *      in      - Input matrix.
 *      theta   - Angle in radians.
 *      out     - Output matrix.
 *
 * Conceptual explanation can be found in the rotation section of the math.c.
 */
void rotate_y(mat4 in, float theta, mat4 out);

/* 
 * Rotates a matrix 'in' by amount theta (in radians) about the z-axis and
 * writes it to 'out'.
 *
 * Parameters:
 *      in      - Input matrix.
 *      theta   - Angle in radians.
 *      out     - Output matrix.
 *
 * Conceptual explanation can be found in the rotation section of the math.c.
 */
void rotate_z(mat4 in, float theta, mat4 out);

/*
 * Wrapper for rotating euler angles stored in a vec3 (in radians) in
 * X -> Y -> Z order and writing it to 'mat'.
 *
 * Parameters:
 *      mat          - input and output matrix.
 *      euler_angles - angles in radians stored as components of a vec3
 */
void rotate_euler_xyz(mat4 mat, vec3 euler_angles);

#endif /* __SAGE_MATH_H__ */
