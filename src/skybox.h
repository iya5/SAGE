#ifndef SAGE_SKYBOX_H
#define SAGE_SKYBOX_H

#include "texture.h"
#include "mesh.h"
#include "mnf/mnf_types.h"

struct skybox {
    struct texture cubemap;
    struct mesh mesh;
};

void skybox_init(struct skybox *skybox, const char *cubemap_paths[6]);
void skybox_draw(struct skybox skybox, mat4 view, mat4 projection);
void skybox_destroy(struct skybox *skybox);

#endif /* SAGE_SKYBOX_H */
