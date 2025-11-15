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

#include "mesh.h"
#include "mnf/mnf_matrix.h"
#include "mnf/mnf_transform.h"
#include "model.h"
#include "shader.h"
#include "mnf/mnf_vector.h"
#include "obj_loader.h"
#include "darray.h"
#include "texture.h"

static void transform_model_matrix(struct transform transform, mat4 out);

struct model model_load_from_file(const char *path)
{
    struct model model;
    struct mesh mesh;

    darray *vertices = obj_load_from_file(path);
    mesh = mesh_create_from_vertices(vertices);
    model.mesh = mesh;

    struct material material = {
        .diffuse_map = texture_create_default(),
        .specular_map = texture_create_default()
    };
    model.material = material;
    
    mnf_vec3_copy(MNF_ONE_VECTOR, model.transform.scale);
    mnf_vec3_copy(MNF_ZERO_VECTOR, model.transform.rotation);
    mnf_vec3_copy(MNF_ZERO_VECTOR, model.transform.position);

    return model;
}

void model_draw(struct model model, struct shader shader)
{
    mat4 model_matrix;
    transform_model_matrix(model.transform, model_matrix);

    mesh_bind(model.mesh);
    shader_uniform_mat4(shader, "u_model", model_matrix);
    mesh_draw(model.mesh);
}

void model_reset_transform(struct model *model) 
{

    mnf_vec3_copy(MNF_ONE_VECTOR, model->transform.scale);
    mnf_vec3_copy(MNF_ZERO_VECTOR, model->transform.rotation);
    mnf_vec3_copy(MNF_ZERO_VECTOR, model->transform.position);
}
void model_scale(struct model *model, vec3 scalars)
{
    mnf_vec3_copy(scalars, model->transform.scale);
}

void model_rotation(struct model *model, vec3 euler_angles)
{
    mnf_vec3_copy(euler_angles, model->transform.rotation);
}

void model_translate(struct model *model, vec3 position)
{
    mnf_vec3_copy(position, model->transform.position);
}

static void transform_model_matrix(struct transform transform, mat4 out)
{
    mnf_mat4_identity(out);
    mnf_mat4_scale(out, transform.scale, out);
    mnf_euler_rotate_x(out, transform.rotation[0], out);
    mnf_euler_rotate_y(out, transform.rotation[1], out);
    mnf_euler_rotate_z(out, transform.rotation[2], out);
    mnf_mat4_translate(out, transform.position, out);
}
