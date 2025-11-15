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

#include <stdint.h>
#include <stdlib.h>
#include <glad/gl.h>

#include "assert.h"
#include "darray.h"
#include "mnf/mnf_types.h"
#include "mnf/mnf_vector.h"
#include "logger.h"
#include "mesh.h"

#define N_VERTICES_2D_TRIANGLE 3
#define N_INDICES_2D_TRIANGLE 3

#define N_VERTICES_QUAD 4
#define N_INDICES_QUAD 6

#define N_VERTICES_CUBE 36

static struct mesh_gpu mesh_gpu_create(const darray *vertices, 
                                       const darray *indices);
static void mesh_gpu_free(struct mesh_gpu *buffer);

struct mesh mesh_create_from_vertices(darray *vertices)
{
    struct mesh mesh;
    mesh.buffer = mesh_gpu_create(vertices, NULL);
    mesh.vertices = vertices;
    mesh.indices = NULL;

    SINFO("Created a mesh with %d vertices", mesh.vertices->len);

    return mesh;
}

struct mesh mesh_geometry_create_cube(void)
{
    struct mesh mesh;

    darray *vertices = darray_alloc(sizeof(struct vertex), 3);

    if (vertices == NULL) goto err;

    vec3 pos[] = {
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}
    };

    vec3 normal[] = {
        { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f},
        { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f, -1.0f},
        { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f},
        { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f,  1.0f},
        {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, {-1.0f,  0.0f,  0.0f},
        { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f},
        { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f},
        { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f},
        { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f, -1.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f}
    };

    vec2 uv[] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}
    };

    for (int i = 0; i < N_VERTICES_CUBE; i++) {
        struct vertex vertex = {0};
        mnf_vec3_copy(pos[i], vertex.pos);
        mnf_vec3_copy(normal[i], vertex.normal);
        mnf_vec2_copy(uv[i], vertex.uv);
        darray_push(vertices, &vertex);
    }

    mesh.buffer = mesh_gpu_create(vertices, NULL);
    mesh.vertices = vertices;
    mesh.indices = NULL;

    return mesh;

err:
    if (vertices) darray_free(vertices);
    SFATAL("Failed to alloc memory for mesh darray");
    exit(1);
}

void mesh_destroy(struct mesh *mesh)
{
    mesh_gpu_free(&(mesh->buffer));

    if (mesh->vertices) {
        darray_free(mesh->vertices);
        mesh->vertices = NULL;
    }

    if (mesh->indices) {
        darray_free(mesh->indices);
        mesh->indices = NULL;
    }
}

void mesh_bind(struct mesh mesh)
{
    glBindVertexArray(mesh.buffer.vao);
    if (mesh.indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffer.ibo);
}

void mesh_draw(struct mesh mesh)
{
    /* this check is probably expensive but is important for my sake */
    int32_t bounded_vao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bounded_vao);
    SASSERT_MSG((int32_t) mesh.buffer.vao == bounded_vao, "Attempted to draw a mesh without binding it first");

    if (mesh.indices) {
        glDrawElements(GL_TRIANGLES, 
                       mesh.buffer.index_count,
                       GL_UNSIGNED_INT,
                       0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh.buffer.vertex_count);
    }
}

static struct mesh_gpu mesh_gpu_create(const darray *vertices, const darray *indices)
{
    struct mesh_gpu buffer;
    
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;

    /* generating buffers */
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    /* bind vao */
    glBindVertexArray(vao);

    /* bind and copy data over to the buffer */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 vertices->item_size * vertices->len,
                 vertices->items,
                 GL_STATIC_DRAW);

    /* configure the vao to interpret attributes */
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


    /* bind normal uv attributes */
    uint32_t normal_index = 1;
    int32_t normal_size = 3;
    void *normal_offset = (void *) ((pos_size) * sizeof(float));
    glVertexAttribPointer(normal_index,
                          normal_size,
                          GL_FLOAT,
                          normalized,
                          stride,
                          normal_offset);

    /* bind texture uv attributes */
    uint32_t uv_index = 2;
    int32_t uv_size = 2;
    void *uv_offset = (void *) ((pos_size + normal_size) * sizeof(float));
    glVertexAttribPointer(uv_index,
                          uv_size,
                          GL_FLOAT,
                          normalized, 
                          stride,
                          uv_offset);
    glEnableVertexAttribArray(uv_index);
    glEnableVertexAttribArray(normal_index);

    if (indices) {
        /* generating buffer for indices */
        glGenBuffers(1, &ibo);
        /* bind indices for index drawing */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices->item_size * indices->len,
                     indices->items,
                     GL_STATIC_DRAW);

        buffer.ibo = ibo;
        buffer.index_count = (uint32_t) indices->len;
    } else {
        buffer.ibo = 0;
        buffer.index_count = 0;
    }


    /* unbinding */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    buffer.vao = vao;
    buffer.vbo = vbo;
    buffer.vertex_count = (uint32_t) vertices->len;

    return buffer;
}

static void mesh_gpu_free(struct mesh_gpu *buffer)
{
    glDeleteVertexArrays(1, &(buffer->vao));
    glDeleteBuffers(1, &(buffer->vbo));
    if (buffer->ibo > 0)
        glDeleteBuffers(1, &(buffer->ibo));

    buffer->vao = 0;
    buffer->vbo = 0;
    buffer->ibo = 0;
    buffer->vertex_count = 0;
    buffer->index_count = 0;
}
