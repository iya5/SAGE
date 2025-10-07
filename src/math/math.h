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

#define X 0
#define Y 1
#define Z 2
#define W 3

#define PI 3.14159
#define DEG_TO_RAD(DEG) ((DEG) * (PI / 180.0))
#define RAD_TO_DEG(RAD) ((RAD) * (180.0 / PI))

#define WOLRD_UP (vec3) {0.0, 1.0, 0.0}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (t)))
#define CLAMP(value, min, max) ((min < max) \
	? (value < min) ? min : ((value > max) ? max : value) \
	: (value < max) ? max : ((value > min) ? min : value))

#endif /* __SAGE_MATH_H__ */
