/* SAGE: Sage Ain't A Game Engine. An OpenGL 3D Renderer

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <glad/gl.h>
#include "skybox.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "mnf/mnf_matrix.h"
#include "logger.h"
#include "obj_loader.h"

struct shader skybox_shader;

void skybox_init(struct skybox *skybox, const char *cubemap_paths[6])
{
    SINFO("Initializing skybox");
    if (skybox_shader.handle == 0)
        skybox_shader = shader_create("glsl/skybox.glsl");

    skybox->mesh = mesh_geometry_create_cube();
    skybox->cubemap = cubemap_texture_create(cubemap_paths);
}

void skybox_draw(struct skybox skybox, mat4 view, mat4 projection)
{
    shader_use(skybox_shader);
    mesh_bind(skybox.mesh);
    cubemap_texture_bind(skybox.cubemap);

    mat4 view_no_translation;
    mnf_mat4_copy(view, view_no_translation);

    view_no_translation[3][0] = 0;
    view_no_translation[3][1] = 0;
    view_no_translation[3][2] = 0;
    view_no_translation[3][3] = 1;

    shader_uniform_mat4(skybox_shader, "u_view", view_no_translation);
    shader_uniform_mat4(skybox_shader, "u_projection", projection);
    glDepthMask(GL_FALSE);
    mesh_draw(skybox.mesh);
    glDepthMask(GL_TRUE);
}

void skybox_destroy(struct skybox *skybox)
{
    SINFO("Destroying skybox");
    texture_destroy(&skybox->cubemap);
    mesh_destroy(&skybox->mesh);
}
