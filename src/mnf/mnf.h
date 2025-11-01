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

/* 
 * this math header defines basic 3d graphics maths used in sage.
 *
 * all operations column-major order and in right-handed coordinates.
 *
 * unless specified otherwise, functions generally take on the form:
 *      out = transform * in
 *
 *  matrix multiplication follows post-order similar to function notation:
 *      m' = m(m)
 *  or, when multiplying a matrix by a vector:
 *      v' = m(v)
 */

#ifndef MANIFOLD_H
#define MANIFOLD_H

#include "mnf_types.h"
#include "mnf_matrix.h"
#include "mnf_transform.h"
#include "mnf_vector.h"
#include "mnf_util.h"
#endif /* MANIFOLD_H */
