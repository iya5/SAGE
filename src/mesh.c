/* Mesh source code for Sage

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

#include "mnf/mnf_matrix.h"
#include "mnf/mnf_transform.h"
#include "mnf/mnf_vector.h"
#include "logger.h"
#include "mesh.h"

struct vertex_array vertex_array_create(const float *vertices, size_t n_bytes);
void vertex_array_free(struct vertex_array *va);

struct mesh mesh_create(const float *vertices, size_t n_bytes)
{
    struct mesh mesh = {0};

    mesh.vertex_array = vertex_array_create(vertices, n_bytes);

    mesh_reset_transform(&mesh);

    /* calculating local transform of mesh */
    mnf_mat4_scale(mesh.model, mesh.transform.scale, mesh.model);
    mnf_euler_rotate_xyz(mesh.model, mesh.transform.rotate, mesh.model);
    mnf_mat4_translate(mesh.model, mesh.transform.position, mesh.model);

    SINFO("Mesh created with %d vertices", mesh.vertex_array.vertex_count);

    return mesh;
}

void mesh_reset_transform(struct mesh *mesh)
{
    mnf_vec3_copy((vec3) {1, 1, 1}, mesh->transform.scale);
    mnf_vec3_copy(MNF_ZERO_VECTOR, mesh->transform.rotate);
    mnf_vec3_copy(MNF_ZERO_VECTOR, mesh->transform.position);
    mnf_mat4_identity(mesh->model);
}

void mesh_set_scale(struct mesh *mesh, vec3 scalars)
{
    mnf_vec3_copy(scalars, mesh->transform.scale);
}

void mesh_set_position(struct mesh *mesh, vec3 position)
{
    mnf_vec3_copy(position, mesh->transform.position);
}

void mesh_set_rotation(struct mesh *mesh, vec3 euler_angles)
{
    mnf_vec3_copy(euler_angles, mesh->transform.rotate);
}

void mesh_update_transform(struct mesh *mesh)
{
    mnf_mat4_identity(mesh->model);
    mnf_mat4_scale(mesh->model, mesh->transform.scale, mesh->model);
    mnf_euler_rotate_xyz(mesh->model, mesh->transform.rotate, mesh->model);
    mnf_mat4_translate(mesh->model, mesh->transform.position, mesh->model);
}

void mesh_destroy(struct mesh *mesh)
{
    vertex_array_free(&(mesh->vertex_array));
}

void mesh_bind(struct mesh mesh)
{
    glBindVertexArray(mesh.vertex_array.vao);
}

void mesh_draw(struct mesh mesh)
{
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_array.vertex_count);
}

struct vertex_array vertex_array_create(const float *vertices, size_t n_bytes)
{
    struct vertex_array va = {0};

    uint32_t vao;
    uint32_t vbo;

    /* generating buffers for vertices */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    /* bind vao */
    glBindVertexArray(vao);

    /* bind and copy data over to the buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, n_bytes, vertices, GL_STATIC_DRAW);

    /* configure the vao to interpret the attributes */
    /* stride is the offset between consecutive generic vertex attributes */
    size_t vertex_size = 8;
    size_t stride = vertex_size * sizeof(float);
    GLboolean normalized = GL_FALSE;

    /* bind position attributes */
    uint32_t pos_index = 0;
    int32_t pos_size = 3;
    void *pos_offset = (void *) 0;
    glVertexAttribPointer(pos_index,
                          pos_size,
                          GL_FLOAT,
                          normalized,
                          stride,
                          pos_offset);
    glEnableVertexAttribArray(pos_index);

    /* bind texture uv attributes */
    uint32_t uv_index = 1;
    int32_t uv_size = 2;
    void *uv_offset = (void *) (pos_size * sizeof(float));
    glVertexAttribPointer(uv_index,
                          uv_size,
                          GL_FLOAT,
                          normalized, 
                          stride,
                          uv_offset);
    glEnableVertexAttribArray(uv_index);

    /* bind normal uv attributes */
    uint32_t normal_index = 2;
    int32_t normal_size = 3;
    void *normal_offset = (void *) ((pos_size + uv_size) * sizeof(float));
    glVertexAttribPointer(normal_index,
                          normal_size,
                          GL_FLOAT,
                          normalized,
                          stride,
                          normal_offset);
    glEnableVertexAttribArray(normal_index);

    /* unbinding */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    va.vao= vao;
    va.vbo = vbo;
    va.vertex_count = (uint32_t) ((n_bytes) / sizeof(float) / 8);

    return va;
}

void vertex_array_free(struct vertex_array *va)
{
    glDeleteVertexArrays(1, &(va->vao));
    glDeleteBuffers(1, &(va->vbo));
    va->vao = 0;
    va->vbo = 0;
    va->vertex_count = 0;
}

void vertex_array_bind(struct vertex_array va)
{
    glBindVertexArray(va.vao);
}

void vertex_array_draw(struct vertex_array va)
{
    glDrawArrays(GL_TRIANGLES, 0, va.vertex_count);
}

