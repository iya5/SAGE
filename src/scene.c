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

#include <stdlib.h>
#include <glad/gl.h>

#include "scene.h"
#include "camera.h"
#include "darray.h"
#include "logger.h"
#include "material.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "lighting.h"

#define LEN_STATIC_ARR(arr) sizeof((arr)) / sizeof((arr[0]))

void scene_init(struct scene *scene, float viewport_width, float viewport_height)
{
    /* Allocating memory for models, materials, lighting, & shaders */
    scene->point_lights = darray_alloc(sizeof(struct point_light), 32);
    scene->models = darray_alloc(sizeof(struct model), 32);

    if (scene->models == NULL || scene->point_lights == NULL) {
        SFATAL("Failed to allocate memory for the scene");
        if (scene->models) darray_free(scene->models);
        if (scene->point_lights) darray_free(scene->point_lights);
        exit(1);
    }

    /* A scene is composed of three important components; camera,
       geometry, & lighting */
    struct camera *cam = &(scene->cam);
    float aspect = viewport_width / viewport_height;
    camera_init(cam, CAM_DEFAULT_POS, CAM_DEFAULT_FORWARD, CAM_DEFAULT_UP);
    camera_perspective(cam,
                       FOV_DEFAULT,
                       aspect,
                       PERSPECTIVE_DEFAULT_NEAR,
                       PERSPECTIVE_DEFAULT_FAR);

    struct shader phong_shader = shader_create("glsl/phong.glsl");

    /* Creating light */
    struct directional_light environment_light = {
        .direction = {0.5, -0.3, 0.5},
        .ambient = {0.0, 0.0, 0.0},
        .diffuse = {0.0, 0.0, 0.0},
        .specular = {0.0, 0.0, 0.0}
    };
    scene->environment_light = environment_light;

    struct point_light point_light = {
        .color = {1.0, 1.0, 1.0},
        .pos = {1.0, 1.0, 1.0},
        .diffuse = {1.0, 1.0, 1.0},
        .specular = {1.0, 1.0, 1.0},
        .constant = 1.0,
        .linear = 0.9,
        .quadratic = 0.032
    };
    darray_push(scene->point_lights, &point_light);

    struct model crab = model_load_from_file("res/crab.obj");
    crab.material = material_create("res/crab.png", NULL, 1024);
    crab.material.shader = phong_shader;
    darray_push(scene->models, &crab);

    /* stick the non-geometric light to the visual lightbulb, syncing their 
       position */
    struct model lightbulb = model_load_from_file("res/sphere.obj");
    lightbulb.material = material_create(NULL, NULL, 0);
    lightbulb.material.shader = phong_shader;
    model_scale(&lightbulb, (vec3){0.2, 0.2, 0.2});
    model_translate(&lightbulb, point_light.pos);
    darray_push(scene->models, &lightbulb);
}

// TODO: refactor
void scene_render(struct scene *scene)
{
    /* clearing buffers */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    struct camera *cam = &(scene->cam);
    camera_update(cam);

    /* rendering models */
    for (uint32_t i = 0; i < scene->models->len; i++) {
        struct model *model = darray_at(scene->models, i);
        struct material material = model->material;
        struct shader shader = material.shader;

        /* pre-rendering model */
        shader_use(shader);
        shader_uniform_mat4(shader, "u_view", cam->view);
        shader_uniform_mat4(shader, "u_projection", cam->projection);
        lighting_apply(shader, scene->environment_light, scene->point_lights);

        /* drawing model */
        material_apply(material);
        model_draw(*model, shader);
    }
}

void scene_destroy(struct scene *scene)
{
    uint32_t n_models = scene->models->len;
    SDEBUG("Destroying %d models", n_models);
    for (uint32_t i = 0; i < scene->models->len; i++) {
        struct model *model = darray_at(scene->models, i);
        model_destroy(model);
    }

    darray_free(scene->point_lights);
    darray_free(scene->models);
}
