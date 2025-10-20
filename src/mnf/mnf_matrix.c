/* Matrix source code for Manifold 

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include "mnf_matrix.h"

void mnf_mat4_copy(mat4 src, mat4 dest)
{
    dest[0][0] = src[0][0]; dest[0][1] = src[0][1];
    dest[1][0] = src[1][0]; dest[1][1] = src[1][1];
    dest[2][0] = src[2][0]; dest[2][1] = src[2][1];
    dest[3][0] = src[3][0]; dest[3][1] = src[3][1];

    dest[0][2] = src[0][2]; dest[0][3] = src[0][3];
    dest[1][2] = src[1][2]; dest[1][3] = src[1][3];
    dest[2][2] = src[2][2]; dest[2][3] = src[2][3];
    dest[3][2] = src[3][2]; dest[3][3] = src[3][3];
}

void mnf_mat4_identity(mat4 mat)
{
    __attribute__((aligned(16)))  mat4 identity = MNF_MAT4_IDENTITY_INIT;
    mnf_mat4_copy(identity, mat);
}

void mnf_mat4_mul(mat4 l, mat4 r, mat4 out)
{
    float a00 = l[0][0], a01 = l[0][1], a02 = l[0][2], a03 = l[0][3];
    float a10 = l[1][0], a11 = l[1][1], a12 = l[1][2], a13 = l[1][3];
    float a20 = l[2][0], a21 = l[2][1], a22 = l[2][2], a23 = l[2][3];
    float a30 = l[3][0], a31 = l[3][1], a32 = l[3][2], a33 = l[3][3];

    float b00 = r[0][0], b01 = r[0][1], b02 = r[0][2], b03 = r[0][3];
    float b10 = r[1][0], b11 = r[1][1], b12 = r[1][2], b13 = r[1][3];
    float b20 = r[2][0], b21 = r[2][1], b22 = r[2][2], b23 = r[2][3];
    float b30 = r[3][0], b31 = r[3][1], b32 = r[3][2], b33 = r[3][3];

    out[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
    out[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
    out[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
    out[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
    out[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
    out[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
    out[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
    out[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
    out[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
    out[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
    out[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
    out[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
    out[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
    out[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
    out[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
    out[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}
