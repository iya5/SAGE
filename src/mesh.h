/* Mesh header for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __SAGE_MESH_H__
#define __SAGE_MESH_H__

#include <stdint.h>
#include <stddef.h>

#include "mnf/mnf_types.h"

struct vertex_array {
    uint32_t vao;
    uint32_t vbo;
    uint32_t vertex_count;
};

struct transform {
    vec3 rotate;
    vec3 scale;
    vec3 position;

};

struct mesh {
    struct vertex_array vertex_array;
    struct transform transform;
    mat4 model;
};

struct mesh mesh_create(const float *vertices, size_t n_bytes);
void mesh_destroy(struct mesh *mesh);
void mesh_bind(struct mesh mesh);
void mesh_draw(struct mesh mesh);

void mesh_reset_transform(struct mesh *mesh);
void mesh_set_scale(struct mesh *mesh, vec3 scalars);
void mesh_reset_transform(struct mesh *mesh);
void mesh_set_rotation(struct mesh *mesh, vec3 euler_angles);
void mesh_set_position(struct mesh *mesh, vec3 position);
void mesh_update_transform(struct mesh *mesh);

#endif /* __SAGE_MESH_H__ */
