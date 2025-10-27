/* manifold graphics math library for sage

this file is part of sage

sage is free software: you can redistribute it and/or modify it under the terms
of the gnu general public license as published by the free software foundation,
either version 3 of the license, or (at your option) any later version.

sage is distributed in the hope that it will be useful, but without any 
warranty; without even the implied warranty of merchantability or fitness for a
particular purpose. see the gnu general public license for more details.

you should have received a copy of the gnu general public license along with 
sage; see the file license. if not, see <https://www.gnu.org/licenses/>.    */

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

#ifndef __mnf_h__
#define __mnf_h__


#include "mnf_types.h"
#include "mnf_matrix.h"
#include "mnf_transform.h"
#include "mnf_vector.h"
#include "mnf_util.h"

#endif /* __mnf_h__ */
