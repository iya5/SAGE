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
    //__attribute__((aligned(16)))  mat4 identity = MNF_MAT4_IDENTITY_INIT;
    mat4 identity = MNF_MAT4_IDENTITY_INIT;
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

void mnf_mat4_mul_vec4(mat4 mat, vec4 in, vec4 out)
{
    float a00 = mat[0][0], a01 = mat[0][1], a02 = mat[0][2], a03 = mat[0][3];
    float a10 = mat[1][0], a11 = mat[1][1], a12 = mat[1][2], a13 = mat[1][3];
    float a20 = mat[2][0], a21 = mat[2][1], a22 = mat[2][2], a23 = mat[2][3];
    float a30 = mat[3][0], a31 = mat[3][1], a32 = mat[3][2], a33 = mat[3][3];

    float x = in[0];
    float y = in[1];
    float z = in[2];
    float w = in[3];

    out[0] = x * a00 + y * a01 + z * a02 + w * a03;
    out[1] = x * a10 + y * a11 + z * a12 + w * a13;
    out[2] = x * a20 + y * a21 + z * a22 + w * a23;
    out[3] = x * a30 + y * a31 + z * a32 + w * a33;
}

void mnf_mat4_inv(mat4 in, mat4 out)
{
    float det = mnf_mat4_det(in);

    /* if the matrix is singular, return identity matrix*/
    if (det == 0.0)
        mnf_mat4_identity(out);

    float a00 = in[0][0], a01 = in[0][1], a02 = in[0][2], a03 = in[0][3];
    float a10 = in[1][0], a11 = in[1][1], a12 = in[1][2], a13 = in[1][3];
    float a20 = in[2][0], a21 = in[2][1], a22 = in[2][2], a23 = in[2][3];
    float a30 = in[3][0], a31 = in[3][1], a32 = in[3][2], a33 = in[3][3];

    float r = 1.0 / det;
    float r00 = r * a00, r01 = r * a01, r02 = r * a02, r03 = r * a03;
    float r10 = r * a10, r11 = r * a11, r12 = r * a12, r13 = r * a13;
    float r20 = r * a20, r21 = r * a21, r22 = r * a22, r23 = r * a23;
    float r30 = r * a30, r31 = r * a31, r32 = r * a32, r33 = r * a33;

    out[0][0] = r00; out[0][1] = r01; out[0][2] = r02; out[0][3] = r03;
    out[1][0] = r10; out[1][1] = r11; out[1][2] = r12; out[1][3] = r13;
    out[2][0] = r20; out[2][1] = r21; out[2][2] = r22; out[2][3] = r23;
    out[3][0] = r30; out[3][1] = r31; out[3][2] = r32; out[3][3] = r33;
}

/* took it from cglm and a reference
 * here: https://allen.in/jee/maths/determinant-of-4-by-4-matrix */
float mnf_mat4_det(mat4 mat)
{
    float t[6];
    float a = mat[0][0], b = mat[0][1], c = mat[0][2], d = mat[0][3],
          e = mat[1][0], f = mat[1][1], g = mat[1][2], h = mat[1][3],
          i = mat[2][0], j = mat[2][1], k = mat[2][2], l = mat[2][3],
          m = mat[3][0], n = mat[3][1], o = mat[3][2], p = mat[3][3];

    t[0] = k * p - o * l;
    t[1] = j * p - n * l;
    t[2] = j * o - n * k;
    t[3] = i * p - m * l;
    t[4] = i * o - m * k;
    t[5] = i * n - m * j;

    return a * (f * t[0] - g * t[1] + h * t[2])
           - b * (e * t[0] - g * t[3] + h * t[4])
           + c * (e * t[1] - f * t[3] + h * t[5])
           - d * (e * t[2] - f * t[4] + g * t[5]);

}

void mnf_mat4_to_mat3(mat4 in, mat3 out)
{
    out[0][0] = in[0][0];
    out[1][0] = in[1][0]; 
    out[2][0] = in[2][0]; 

    out[0][1] = in[0][1];
    out[1][1] = in[1][1];
    out[2][1] = in[2][1];

    out[0][2] = in[0][2]; 
    out[1][2] = in[1][2];
    out[2][2] = in[2][2];
}
