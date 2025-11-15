/* SAGE: Sage Ain't A Game Engine. An OpenGL 3D Renderer.

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <stdio.h>
#include <string.h>

#include "lighting.h"

#define MAX_UNIFORM_NAME_LEN 64

void set_light_params(struct shader shader,
                      struct directional_light directional_light,
                      struct material material,
                      darray *point_lights)
{
    /* setting directional light */
    shader_uniform_vec3(shader, "u_directional_light.direction", directional_light.direction);
    shader_uniform_vec3(shader, "u_directional_light.ambient", directional_light.ambient);
    shader_uniform_vec3(shader, "u_directional_light.diffuse", directional_light.diffuse);
    shader_uniform_vec3(shader, "u_directional_light.specular", directional_light.specular);

    /* setting point lights */
    for (size_t i = 0; i < point_lights->len; i++) {
        struct point_light *point_light = darray_at(point_lights, i);
        char uniform_name[MAX_UNIFORM_NAME_LEN] = {0};

        /* position */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].pos", i);
        shader_uniform_vec3(shader, uniform_name, point_light->pos);
        memset(uniform_name, 0, MAX_UNIFORM_NAME_LEN);

        /* diffuse */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].diffuse", i);
        shader_uniform_vec3(shader, uniform_name, point_light->diffuse);
        memset(uniform_name, 0, MAX_UNIFORM_NAME_LEN);

        /* specular */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].specular", i);
        shader_uniform_vec3(shader, uniform_name, point_light->specular);
        memset(uniform_name, 0, MAX_UNIFORM_NAME_LEN);

        /* constant */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].constant", i);
        shader_uniform_1f(shader, uniform_name, point_light->constant);
        memset(uniform_name, 0, MAX_UNIFORM_NAME_LEN);
        /* linear */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].linear", i);
        shader_uniform_1f(shader, uniform_name, point_light->linear);
        memset(uniform_name, 0, MAX_UNIFORM_NAME_LEN);

        /* quadratic */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].quadratic", i);
        shader_uniform_1f(shader, uniform_name, point_light->quadratic);
        memset(uniform_name, 0, MAX_UNIFORM_NAME_LEN);
    }
    shader_uniform_1f(shader, "u_material.shininess", material.shininess);

    shader_uniform_1i(shader, "u_num_point_lights", 2);
}
