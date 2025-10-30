/* Utilities for Manifold 

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __MNF_UTIL_H__
#define __MNF_UTIL_H__

#define PI 3.14159f
#define MNF_RAD(DEG) ((DEG) * (PI / 180.0f))
#define MNF_DEG(RAD) ((RAD) * (180.0f / PI))

#define WOLRD_UP (vec3){0.0f, 1.0f, 0.0f}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAMP(value, min, max) ((min < max) \
	? (value < min) ? min : ((value > max) ? max : value) \
	: (value < max) ? max : ((value > min) ? min : value))

#endif /* __MNF_UTIL_H__ */
