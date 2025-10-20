/* Transform header for Manifold 

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __MNF_TRANSFORM_H__
#define __MNF_TRANSFORM_H__

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

#endif /* __MNF_TRANSFORM_H__ */

