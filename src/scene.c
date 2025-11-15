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
#include "model.h"
#include "shader.h"
#include "texture.h"

#define LEN_STATIC_ARR(arr) sizeof((arr)) / sizeof((arr[0]))

struct model crab;
struct shader shader;

void scene_init(struct scene *scene, float viewport_width, float viewport_height)
{
    /* Allocating memory for models, materials, lighting, & shaders */
    scene->models = darray_alloc(sizeof(struct model), 32);
    scene->point_lights = darray_alloc(sizeof(struct model), 32);

    if (scene->models == NULL || scene->point_lights == NULL) {       SFATAL("Failed to allocate memory for the scene");
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

    /* Creating light */
    struct directional_light environment_light = {
        .direction = {0.5, -0.3, 0.5},
        .ambient = {0.3, 0.3, 0.3},
        .diffuse = {0.0, 0.0, 0.0},
        .specular = {0.0, 0.0, 0.0}
    };
    scene->environment_light = environment_light;

    struct shader color = shader_create("glsl/color.glsl");

    crab = model_load_from_file("res/crab.obj");
    shader = color;
}

// TODO: refactor
void scene_render(struct scene *scene)
{
    /* pre-rendering (clearing buffers) */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    struct camera *cam = &(scene->cam);
    camera_update(cam);

    /* drawing */
    shader_use(shader);
    model_reset_transform(&crab);
    shader_uniform_mat4(shader, "u_view", cam->view);
    shader_uniform_vec4(shader, "u_color", (vec4){1.0, 1.0, 1.0, 1.0});
    shader_uniform_mat4(shader, "u_projection", cam->projection);
    model_draw(crab, shader);

    model_reset_transform(&crab);
    model_translate(&crab, (vec3){2.0, 5.0, 0.0});
    model_draw(crab, shader);


    /* disabling to test single object only
    for (uint32_t i = 0; i < scene->models->len; i++) {
        struct model *model = darray_at(scene->models, i);
        struct material material = model->material;
        struct shader shader = material.shader;

        shader_use(material.shader);
        shader_uniform_mat4(shader, "u_view", cam->view);
        shader_uniform_mat4(shader, "u_projection", cam->projection);

        model_draw(*model, shader);
    }
    */
}

void scene_destroy(struct scene *scene)
{
    darray_free(scene->point_lights);
    darray_free(scene->models);
}
