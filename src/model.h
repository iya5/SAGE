#ifndef SAGE_MODEL_H
#define SAGE_MODEL_H

#include <stdbool.h>

#include "material.h"
#include "shader.h"
#include "mesh.h"

#define MODEL_NAME_MAX_SIZE 64

struct transform {
    vec3 rotation;
    vec3 scale;
    vec3 position;
};

struct model {
    char name[MODEL_NAME_MAX_SIZE];
    struct mesh mesh;
    struct material material;
    struct transform transform;
    bool visible;
};

struct model model_load_from_file(const char *path);
void model_set_name(struct model *model, char name[MODEL_NAME_MAX_SIZE]);
struct model model_create_cube(void);
void model_draw(struct model model, struct shader shader);
void model_destroy(struct model *model);

void model_reset_transform(struct model *model);
void model_scale(struct model *model, vec3 scalars);
void model_rotation(struct model *model, vec3 euler_angles);
void model_translate(struct model *model, vec3 position);

#endif /* SAGE_MODEL_H */
