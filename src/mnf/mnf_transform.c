#include <math.h>

#include "mnf_matrix.h"
#include "mnf_transform.h"
#include "mnf_vector.h"

void mnf_euler_rotate_x(mat4 in, float phi, mat4 out)
{
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
