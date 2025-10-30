/* Matrix header for Manifold 

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __MNF_MATRIX_H__
#define __MNF_MATRIX_H__

#include "mnf_types.h"

#define MNF_MAT4_IDENTITY_INIT {{1.0, 0.0, 0.0, 0.0}, \
                                {0.0, 1.0, 0.0, 0.0}, \
                                {0.0, 0.0, 1.0, 0.0}, \
                                {0.0, 0.0, 0.0, 1.0}}

#define MNF_MAT4_ZEROINIT {{0.0, 0.0, 0.0, 0.0}, \
                           {0.0, 0.0, 0.0, 0.0}, \
                           {0.0, 0.0, 0.0, 0.0}, \
                           {0.0, 0.0, 0.0, 0.0}}

/* Copies 4x4 src to 4x4 dest. */
void mnf_mat4_copy(mat4 src, mat4 dest);

/* Sets the 4x4 matrix into an identity matrix. */
void mnf_mat4_identity(mat4 mat);

/* 
 * Multiplies two matrices and writing the result to 'out'.
 *
 * This takes the form following right to left order
 *      out = left * right
 *
 * Both left and right can be the output matrix.
 */
void mnf_mat4_mul(mat4 left, mat4 right, mat4 out);

/* Retrieves the upper left 3x3 in a mat4 and writing it to 'out' */
void mnf_mat4_to_mat3(mat4 in, mat3 out);


#endif /* __MNF_MATRIX_H__ */
