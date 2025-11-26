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
