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

#include "mnf/mnf_vector.h"
#include "skybox.h"
#include "scene.h"
#include "camera.h"
#include "darray.h"
#include "logger.h"
#include "material.h"
#include "model.h"
#include "shader.h"
#include "lighting.h"

static void scene_clear_color(struct scene *scene);

struct shader phong_shader;
struct shader light_shader;

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

    scene->draw_skybox = true;
    mnf_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, scene->clear_color);

    /* preparing shaders */
    phong_shader = shader_create("glsl/phong.glsl");
    light_shader = shader_create("glsl/light.glsl");

    /* A scene is composed of three important components; camera,
       geometry, & lighting */

    /* This section consists of the camera */
    struct camera *cam = &(scene->cam);
    float aspect = viewport_width / viewport_height;
    camera_init(cam, CAM_DEFAULT_POS, CAM_DEFAULT_FORWARD, CAM_DEFAULT_UP);
    camera_perspective(cam,
                       FOV_DEFAULT,
                       aspect,
                       PERSPECTIVE_DEFAULT_NEAR,
                       PERSPECTIVE_DEFAULT_FAR);

    scene->lighting_params.enable_ambient = true;
    scene->lighting_params.enable_diffuse = true;
    scene->lighting_params.enable_specular = true;
    scene_init_lighting(scene);
    scene_init_models(scene);
    scene_init_skybox(scene);

    SINFO("Finished Initializing Scene!");
}

void scene_render(struct scene *scene)
{
    scene_clear_color(scene);

    struct camera *cam = &(scene->cam);
    camera_update(cam);

    if (scene->draw_skybox) skybox_draw(scene->skybox, cam->view, cam->projection);

    shader_use(light_shader);
    shader_uniform_mat4(light_shader, "u_view", cam->view);
    shader_uniform_mat4(light_shader, "u_projection", cam->projection);
    for (uint32_t i = 0; i < scene->point_lights->len; i++) {
        struct point_light *light = darray_at(scene->point_lights, i);
        if (!light->visible) continue;

        struct model light_model = light->geometric_model;
        model_translate(&light_model, light->pos);
        shader_uniform_vec3(light_shader, "u_color", light->color);
        model_draw(light_model, light_shader);
    }

    shader_use(phong_shader);
    shader_uniform_mat4(phong_shader, "u_view", cam->view);
    shader_uniform_vec3(phong_shader, "u_view_pos", cam->pos);
    shader_uniform_mat4(phong_shader, "u_projection", cam->projection);
    for (uint32_t i = 0; i < scene->models->len; i++) {
        struct model *model = darray_at(scene->models, i);
        if (!model->visible) continue;

        lighting_apply(phong_shader,
                       scene->environment_light,
                       scene->point_lights,
                       scene->lighting_params);

        material_apply(phong_shader, model->material);
        model_draw(*model, phong_shader);
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
    shader_destroy(&phong_shader);
    shader_destroy(&light_shader);
}

static void scene_clear_color(struct scene *scene)
{
    /* clearing buffers */
    glClearColor(scene->clear_color[0], scene->clear_color[1], scene->clear_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
