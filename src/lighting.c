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
#include "logger.h"

#define MAX_UNIFORM_NAME_LEN 64

void lighting_apply(struct shader active_shader,
                    struct directional_light directional_light,
                    darray *point_lights)
{
    shader_uniform_vec3(active_shader, "u_directional_light.direction", directional_light.direction);
    shader_uniform_vec3(active_shader, "u_directional_light.ambient", directional_light.ambient);
    shader_uniform_vec3(active_shader, "u_directional_light.diffuse", directional_light.diffuse);
    shader_uniform_vec3(active_shader, "u_directional_light.specular", directional_light.specular);

    shader_uniform_1i(active_shader, "u_num_point_lights", point_lights->len);

    for (size_t i = 0; i < point_lights->len; i++) {
        struct point_light *point_light = darray_at(point_lights, i);
        char uniform_name[MAX_UNIFORM_NAME_LEN] = {0};

        /* visible */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].visible", i);
        shader_uniform_1i(active_shader, uniform_name, point_light->visible);
        if (!point_light->visible) continue;

        /* position */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].pos", i);
        shader_uniform_vec3(active_shader, uniform_name, point_light->pos);

        /* diffuse */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].diffuse", i);
        shader_uniform_vec3(active_shader, uniform_name, point_light->diffuse);

        /* specular */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].specular", i);
        shader_uniform_vec3(active_shader, uniform_name, point_light->specular);

        /* constant */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].constant", i);
        shader_uniform_1f(active_shader, uniform_name, point_light->constant);
        /* linear */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].linear", i);
        shader_uniform_1f(active_shader, uniform_name, point_light->linear);

        /* quadratic */
        snprintf(uniform_name, MAX_UNIFORM_NAME_LEN, "u_point_lights[%zu].quadratic", i);
        shader_uniform_1f(active_shader, uniform_name, point_light->quadratic);

    }
}

void light_set_name(struct point_light *light, char name[LIGHT_NAME_MAX_SIZE])
{
    strncpy(light->name, name, LIGHT_NAME_MAX_SIZE);
}

