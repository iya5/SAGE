#include "mnf_vector.h"
#include <math.h>

void mnf_vec3_copy(vec3 src, vec3 dest)
{
    dest[X] = src[X];
    dest[Y] = src[Y];
    dest[Z] = src[Z];
}


void mnf_vec2_copy(vec2 src, vec2 dest)
{
    dest[X] = src[X];
    dest[Y] = src[Y];
}

void mnf_vec3_add(vec3 left, vec3 right, vec3 out)
{
    out[X] = left[X] + right[X];
    out[Y] = left[Y] + right[Y];
    out[Z] = left[Z] + right[Z];
}


void mnf_vec3_sub(vec3 left, vec3 right, vec3 out)
{
    out[X] = left[X] - right[X];
    out[Y] = left[Y] - right[Y];
    out[Z] = left[Z] - right[Z];
}

float mnf_vec3_norm(vec3 v)
{
    float vx2 = v[X] * v[X];
    float vy2 = v[Y] * v[Y];
    float vz2 = v[Z] * v[Z];
    
    return sqrt(vx2 + vy2 + vz2);
}

void mnf_vec3_normalize(vec3 in, vec3 out)
{
    float norm = mnf_vec3_norm(in);
    if (norm == 0.0) {
        out[X] = 0;
        out[Y] = 0;
        out[Z] = 0;
        return;
    }

    out[X] = in[X] / norm;
    out[Y] = in[Y] / norm;
    out[Z] = in[Z] / norm;
}

void mnf_vec3_scale(vec3 in, float scalar, vec3 out)
{
    out[X] = in[X] * scalar;
    out[Y] = in[Y] * scalar;
    out[Z] = in[Z] * scalar;
}

float mnf_vec3_dot(vec3 left, vec3 right)
{
    float axbx = left[X] * right[X];
    float ayby = left[Y] * right[Y];
    float azbz = left[Z] * right[Z];

    return axbx + ayby + azbz;
}

void mnf_vec3_cross(vec3 left, vec3 right, vec3 out)
{
    float aybz = left[Y] * right[Z], azby = left[Z] * right[Y];
    float azbx = left[Z] * right[X], axbz = left[X] * right[Z];
    float axby = left[X] * right[Y], aybx = left[Y] * right[X];

    out[X] = aybz - azby;
    out[Y] = azbx - axbz;
    out[Z] = axby - aybx;
}
