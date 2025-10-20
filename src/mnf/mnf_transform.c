/* Transform source code for Manifold

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <math.h>

#include "mnf_matrix.h"
#include "mnf_transform.h"
#include "mnf_vector.h"

void mnf_euler_rotate_x(mat4 in, float phi, mat4 out)
{
    /*
     * For rotating around the x-axis
     * [1    0     0 ]
     * [0  cosφ -sinφ]
     * [0  sinφ  cosφ]
     */
    mat4 rotate_x = {
        {1,         0,         0, 0},
        {0,  cos(phi),  sin(phi), 0},
        {0, -sin(phi),  cos(phi), 0},
        {0,        0 ,         0, 1},
    };

    mnf_mat4_mul(rotate_x, in, out);

}

void mnf_euler_rotate_y(mat4 in, float theta, mat4 out)
{
    /* 
     * For rotating around the y-axis
     * [cosθ 0 -sinθ]
     * [ 0   1   0  ]
     * [sinθ 0  cosθ]
     */
    mat4 rotate_y = {
        { cos(theta), 0,  sin(theta), 0},
        {          0, 1,           0, 0},
        {-sin(theta), 0,  cos(theta), 0},
        {          0, 0,           0, 1},
    };

    mnf_mat4_mul(rotate_y, in, out);
}

void mnf_euler_rotate_z(mat4 in, float psi, mat4 out)
{
    /*
     * For rotating around the z-axis
     * [cosψ -sinψ 0]
     * [sinψ  cosψ 0]
     * [ 0     0   1]
     */
    mat4 rotate_z = {
        { cos(psi), sin(psi), 0, 0},
        {-sin(psi), cos(psi), 0, 0},
        {        0,        0, 1, 0},
        {        0,        0, 0, 1},
    };

    mnf_mat4_mul(rotate_z, in, out);
}

void mnf_euler_rotate_xyz(mat4 in, vec3 euler_angles, mat4 out)
{
    mat4 rotation;
    mnf_mat4_identity(rotation);
    mnf_euler_rotate_x(rotation, euler_angles[X], rotation);
    mnf_euler_rotate_y(rotation, euler_angles[Y], rotation);
    mnf_euler_rotate_z(rotation, euler_angles[Z], rotation);
    mnf_mat4_mul(rotation, in, out);
}

void mnf_axis_angle_rotate(mat4 in, vec3 n, float theta, mat4 out)
{
    // TODO: add formula here
    // in column major
    mnf_vec3_normalize(n, n);

    float nx = n[X];
    float ny = n[Y];
    float nz = n[Z];

    float nx2 = nx * nx;
    float ny2 = ny * ny;
    float nz2 = nz * nz;

    float nxny = nx * ny;
    float nxnz = nx * nz;
    float nynz = ny * nz;

    float cosA = cos(theta);
    float sinA = sin(theta);

    float k = 1 - cosA;

    mat4 axis_rotate = {
        {nx2*k + cosA, nxny*k + nx*sinA, nxnz*k - ny*sinA, 0},
        {nxny*k - nz*sinA, ny2*k + cosA, nynz*k + nx*sinA, 0},
        {nxnz*k + ny*sinA, nynz*k - nx*sinA, nz2*k + cosA, 0},
        {0, 0, 0, 1}
    };

    mnf_mat4_mul(axis_rotate, in, out);
}


void mnf_mat4_scale(mat4 in, vec3 scalars, mat4 out)
{
    /* 
     * For scaling a mat4 based on a vec3 of scalars k
     * [1 0 0 0][kx]   [kx  0  0 0]
     * [0 1 0 0][ky] = [ 0 ky  0 0]
     * [0 0 1 0][kz]   [ 0  0 kz 0]
     * [0 0 0 1][ 1]   [ 0  0  0 1]
     */

    mat4 scale = {
        {scalars[X], 0, 0, 0},
        {0, scalars[Y], 0, 0},
        {0, 0, scalars[Z], 0},
        {0, 0, 0, 1},
    };

    mnf_mat4_mul(scale, in, out);
}

void mnf_mat4_scale_along_axis(mat4 in, float scalar, vec3 n, mat4 out)
{
    /*
     * For scaling along the axis parallel to n (as a unit vector) by scalar k
     * [1+(k-1)nx²  (k-1)nxny  (k-1)nxnz 0]
     * [ (k-1)nxny 1+(k-1)ny²  (k-1)nynz 0]
     * [ (k-1)nxnz  (k-1)nynz 1+(k-1)nz² 0]
     * [         0          0          0 1]
     */
    
    float nx2 = n[X] * n[X];
    float ny2 = n[Y] * n[Y];
    float nz2 = n[Z] * n[Z];

    float nxny = n[X] * n[Y];
    float nxnz = n[X] * n[Z];
    float nynz = n[Y] * n[Z];

    float k = scalar - 1;

    mat4 scale_axis = {
        {1+k*nx2, k*nxny, k*nxnz, 0},
        {k*nxnz, 1+k*ny2, k*nynz, 0},
        {k*nxnz, k*nynz, 1+k*nz2, 0},
        {0, 0, 0, 1}
    };

    mnf_mat4_mul(scale_axis, in, out);
}

void mnf_mat4_translate(mat4 in, vec3 v, mat4 out)
{
    mat4 translate;
    mnf_mat4_identity(translate);
    translate[3][0] = v[X];
    translate[3][1] = v[Y];
    translate[3][2] = v[Z];
    mnf_mat4_mul(translate, in, out);
}
