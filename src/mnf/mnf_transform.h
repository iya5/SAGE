#ifndef MANIFOLD_TRANSFORM_H
#define MANIFOLD_TRANSFORM_H

#include "mnf_types.h"

/* Rotates a mat4 around the x-axis by phi radians, writing it to 'out' */
void mnf_euler_rotate_x(mat4 in, float phi, mat4 out);

/* Rotates a mat4 around the y-axis by theta radians, writing it to 'out' */
void mnf_euler_rotate_y(mat4 in, float theta, mat4 out);

/* Rotates a mat4 around the z-axis by psi radians, writing it to 'out' */
void mnf_euler_rotate_z(mat4 in, float psi, mat4 out);

/* 
 * Rotates a mat4 using euler angles in a vec3 in the order of XYZ, writing it
 * to 'out'
 */ 
void mnf_euler_rotate_xyz(mat4 in, vec3 euler_angles, mat4 out);

/* 
 * Rotates a mat4 around an arbitrary axis by a vec3 of euler angles, writing it
 * to 'out'
 */
void mnf_axis_angle_rotate(mat4 in, vec3 n, float theta, mat4 out);


void mnf_mat4_translate(mat4 in, vec3 v, mat4 out);


void mnf_mat4_scale(mat4 in, vec3 scalars, mat4 out);

#endif /* MANIFOLD_TRANSFORM_H */

