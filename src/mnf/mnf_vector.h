/* Vector header for Manifold 

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __MNF_VECTOR_H__
#define __MNF_VECTOR_H__

#include "mnf_types.h"

#define MNF_ZERO_VECTOR (vec3){0, 0, 0}

/* Copies a vec3 src to vec3 dest */
void mnf_vec3_copy(vec3 src, vec3 dest);

/* Adds two vec3 a and b, writing the sum to 'out' */
void mnf_vec3_add(vec3 left, vec3 right, vec3 out);

/* Subtracts two vec3 left and right, writing the difference to 'out' */
void mnf_vec3_sub(vec3 left, vec3 right, vec3 out);

/* Returns the Euclidean norm (magnitude) of a vec3. */
float mnf_vec3_norm(vec3 v);

/* Normalizes a vec3, writing the result to 'out'. */
void mnf_vec3_normalize(vec3 in, vec3 out);

/* Scales a vector by a scalar, writing the result to 'out'. */
void mnf_vec3_scale(vec3 in, float scalar, vec3 out);

/* Returns the dot product (scalar) of vec3 a and b. */
float mnf_vec3_dot(vec3 left, vec3 right);

/* Gets the cross product (vector) of vec3 a and b, writing it to 'out'. */
void mnf_vec3_cross(vec3 left, vec3 right, vec3 out);

#endif /* __MNF_VECTOR_H__ */
