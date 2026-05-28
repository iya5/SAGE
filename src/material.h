#ifndef SAGE_MATERIAL_H
#define SAGE_MATERIAL_H

#include "shader.h"
#include "texture.h"


struct material {
    struct texture diffuse_map;
    struct texture specular_map;
    float shininess;
};

struct material material_create_default(void);

struct material material_create(const char *diffuse_map_path,
                                const char *specular_map_path,
                                float shininess);

void material_apply(struct shader shader, struct material material);

#endif /* SAGE_MATERIAL_H */
