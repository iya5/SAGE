#ifndef SAGE_MESH_H
#define SAGE_MESH_H

#include <stdint.h>
#include <stddef.h>

#include "mnf/mnf_types.h"
#include "darray.h"

struct vertex {
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

struct mesh_gpu {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;
    uint32_t vertex_count;
    uint32_t index_count;
};

struct mesh {
    struct mesh_gpu buffer;
    darray *vertices;
    darray *indices;
};


/* TODO: documentation */

/* ... */
struct mesh mesh_geometry_create_cube(void);
/* ... */
struct mesh mesh_create(darray *vertices, darray *indices);
/* ... */
void mesh_destroy(struct mesh *mesh);
/* ... */
void mesh_bind(struct mesh mesh);
/* ... */
void mesh_draw(struct mesh mesh);

#endif /* SAGE_MESH_H */
