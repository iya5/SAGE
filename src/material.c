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

#include "material.h"

void lighting_model_set_params(vec3 ambient,
                               struct light light,
                               struct material material,
                               struct shader shader)
{
    if (light.type == LIGHT_POINT)
        shader_uniform_1i(shader, "u_flat_shading", 1);
    else
        shader_uniform_1i(shader, "u_flat_shading", 0);

    /* set light parameters of the light equation */
    shader_uniform_vec3(shader, "u_light.pos", light.pos);
    shader_uniform_vec3(shader, "u_light.ambient", ambient);
    shader_uniform_vec3(shader, "u_light.diffuse", light.diffuse);
    shader_uniform_vec3(shader, "u_light.specular", light.specular);


    /* set surface parameters of the light equation */
    //shader_uniform_vec3(shader, "u_material.ambient", material.ambient);
    //shader_uniform_vec3(shader, "u_material.diffuse", material.diffuse);
    //shader_uniform_vec3(shader, "u_material.specular", material.specular);
    shader_uniform_1f(shader, "u_material.shininess", material.shininess);
}
