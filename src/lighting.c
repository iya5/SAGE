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
#include "mnf/mnf_vector.h"

#define MAX_UNIFORM_NAME_LEN 64

struct point_light point_light_create(const char name[LIGHT_NAME_MAX_SIZE], float attenuation_range)
{
    struct point_light light = {
        .color = {1.0f, 1.0f, 1.0f},
        .pos = {0.0f, 0.0f, 0.0f},
        .diffuse = {1.0f, 1.0f, 1.0f},
        .specular = {1.0f, 1.0f, 1.0f},
        .visible = true,
        .attenuation_range = attenuation_range
    };

    light_set_name(&light, name);
    point_light_set_attenuation_range(&light, attenuation_range);

    return light;
}

void point_light_set_pos(struct point_light *light, vec3 pos)
{
    mnf_vec3_copy(pos, light->pos);
}

void point_light_set_color(struct point_light *light, vec3 color)
{
    mnf_vec3_copy(color, light->color);
}

void point_light_set_diffuse(struct point_light *light, vec3 diffuse)
{
    mnf_vec3_copy(diffuse, light->diffuse);
}

void point_light_set_specular(struct point_light *light, vec3 specular)
{
    mnf_vec3_copy(specular, light->specular);
}

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

void light_set_name(struct point_light *light, const char name[LIGHT_NAME_MAX_SIZE])
{
    strncpy(light->name, name, LIGHT_NAME_MAX_SIZE);
}

void point_light_set_attenuation_range(struct point_light *light, float range)
{
    /* From https://wiki.ogre3d.org/tiki-index.php?page=-Point%20Light%20Attenuation&structure=Ogre%20Lexicon
       Range Constant Linear Quadratic
       3250, 1.0, 0.0014, 0.000007
       600, 1.0, 0.007, 0.0002
       325, 1.0, 0.014, 0.0007
       200, 1.0, 0.022, 0.0019
       160, 1.0, 0.027, 0.0028
       100, 1.0, 0.045, 0.0075
       65, 1.0, 0.07, 0.017
       50, 1.0, 0.09, 0.032
       32, 1.0, 0.14, 0.07
       20, 1.0, 0.22, 0.20
       13, 1.0, 0.35, 0.44
       7, 1.0, 0.7, 1.8

       Changing the range won't dynamically change the actual attenuation but
       will be chose based on the break points instead. This can just be thought
       of as a look-up table and is pre-calculated by somebody already */
    light->constant = 1.0;
    if (range >= 3250) {
        light->linear = 0.0014;
        light->quadratic = 0.000007;
    } else if (range >= 600) {
        light->linear = 0.007;
        light->quadratic = 0.0002;
    } else if (range >= 325) {
        light->linear = 0.014;
        light->quadratic = 0.0007;
    } else if (range >= 200) {
        light->linear = 0.022;
        light->quadratic = 0.0019;
    } else if (range >= 160) {
        light->linear = 0.027;
        light->quadratic = 0.0028;
    } else if (range >= 100) {
        light->linear = 0.045;
        light->quadratic = 0.0075;
    } else if (range >= 65) {
        light->linear = 0.07;
        light->quadratic = 0.017;
    } else if (range >= 50) {
        light->linear = 0.09;
        light->quadratic = 0.032;
    } else if (range >= 32) {
        light->linear = 0.14;
        light->quadratic = 0.07;
    } else if (range >= 20) {
        light->linear = 0.22;
        light->quadratic = 0.20;
    } else if (range >= 13) {
        light->linear = 0.35;
        light->quadratic = 0.44;
    } else {
        light->linear = 0.7;
        light->quadratic = 1.8;
    }
}
