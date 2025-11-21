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
#include "lighting.h"
#include "skybox.h"

struct skybox skybox;

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
    struct shader light_shader = shader_create("glsl/light.glsl");

    struct model sphere = model_load_from_file("res/sphere.obj");
    sphere.material = material_create("res/textures/base.png", NULL, 32);
    sphere.material.shader = phong_shader;
    model_scale(&sphere, (vec3){0.5, 0.5, 0.5});
    model_translate(&sphere, (vec3){3, 0.0, -1.0});
    model_set_name(&sphere, "Sphere");
    darray_push(scene->models, &sphere);

    struct model crab = model_load_from_file("res/crab.obj");
    crab.material = material_create("res/crab.png", NULL, 10);
    crab.material.shader = phong_shader;
    model_set_name(&crab, "Crab");
    darray_push(scene->models, &crab);

    struct model avocado = model_load_from_file("res/avocado.obj");
    avocado.material = material_create("res/avocado/textures/avocado_albedo.jpeg", 
                                    "res/avocado/textures/avocado_specular.jpeg", 1);
    avocado.material.shader = phong_shader;
    model_scale(&avocado, (vec3){10, 10, 10});
    model_translate(&avocado, (vec3){-3, 2, 1});
    model_set_name(&avocado, "Avocado");
    darray_push(scene->models, &avocado);
    
    struct model croissant = model_load_from_file("res/croissant.obj");
    croissant.material = material_create("res/croissant/textures/croissant_albedo.jpeg", 
                                        "res/croissant/textures/croissant_albedo.jpeg", 1);
    croissant.material.shader = phong_shader;
    model_scale(&croissant, (vec3){10, 10, 10});
    model_translate(&croissant, (vec3){-1, 2, 1});
    model_set_name(&croissant, "Croissant");
    darray_push(scene->models, &croissant);
    
    struct model lemon = model_load_from_file("res/lemon.obj");
    lemon.material = material_create("res/lemon/textures/lemon_albedo.jpeg", 
                                    "res/lemon/textures/lemon_specular.jpeg", 1);
    lemon.material.shader = phong_shader;
    model_scale(&lemon, (vec3){10, 10, 10});
    model_translate(&lemon, (vec3){1, 2, 1});
    model_set_name(&lemon, "Lemon");
    darray_push(scene->models, &lemon);

    struct model lime = model_load_from_file("res/lime.obj");
    lime.material = material_create("res/lime/textures/lime_albedo.jpeg", 
                                    "res/lime/textures/lime_specular.jpeg", 1);
    lime.material.shader = phong_shader;
    model_scale(&lime, (vec3){10, 10, 10});
    model_translate(&lime, (vec3){2, 2, 1});
    model_set_name(&lime, "Lime");
    darray_push(scene->models, &lime);
    
    struct model orange = model_load_from_file("res/orange.obj");
    orange.material = material_create("res/orange/textures/orange_albedo.jpeg", 
                                    "res/orange/textures/orange_specular.jpeg", 1);
    orange.material.shader = phong_shader;
    model_scale(&orange, (vec3){10, 10, 10});
    model_translate(&orange, (vec3){4, 2, 1});
    model_set_name(&orange, "Orange");
    darray_push(scene->models, &orange);
    

    struct model light_body = model_load_from_file("res/sphere.obj");
    light_body.material = material_create(NULL, NULL, 1);
    light_body.material.shader = light_shader;
    model_scale(&light_body, (vec3){0.1, 0.1, 0.1});

    struct model floor = model_create_cube();
    floor.material = material_create("res/textures/base.png", NULL, 1);
    floor.material.shader = phong_shader;
    model_scale(&floor, (vec3){30, 0.1, 30});
    model_translate(&floor, (vec3){0, -2, 0});
    model_set_name(&floor, "Floor");
    darray_push(scene->models, &floor);

    /* Creating light */
    struct directional_light environment_light = {
        .direction = {0, -1.0, -1.0},
        .ambient = {0.8, 0.8, 0.8},
        .diffuse = {0.0, 0.0, 0.0},
        .specular = {0.0, 0.0, 0.0}
    };
    scene->environment_light = environment_light;

    struct point_light r_light = {
        .color = {1.0, 0.0, 0.0},
        .pos = {1.0, 4.0, 0.0},
        .diffuse = {1.0, 0.0, 0.0},
        .specular = {1.0, 0.0, 0.0},
        .constant = 1.0,
        .linear = 0.9,
        .quadratic = 0.032,
        .geometric_model = light_body,
        .visible = true,
    };
    light_set_name(&r_light, "Point Light 1");
    darray_push(scene->point_lights, &r_light);


    struct point_light g_light = {
        .color = {0.0, 1.0, 0.0},
        .pos = {-2.0, 4.0, 2.0},
        .diffuse = {0.0, 1.0, 0.0},
        .specular = {0.0, 1.0, 0.0},
        .constant = 1.0,
        .linear = 0.9,
        .quadratic = 0.032,
        .geometric_model = light_body,
        .visible = true
    };
    light_set_name(&g_light, "Point Light 2");
    darray_push(scene->point_lights, &g_light);


    struct point_light b_light = {
        .color = {0.0, 0.0, 1.0},
        .pos = {3.0, 4.0, -3.0},
        .diffuse = {0.0, 0.0, 1.0},
        .specular = {0.0, 0.0, 1.0},
        .constant = 1.0,
        .linear = 0.9,
        .quadratic = 0.032,
        .geometric_model = light_body,
        .visible = true
    };
    light_set_name(&b_light, "Point Light 3");
    darray_push(scene->point_lights, &b_light);

     const char *cubemap_faces[6] = {
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg",
    };
    skybox_init(&skybox, cubemap_faces);
}


// TODO: refactor
void scene_render(struct scene *scene)
{
    /* clearing buffers */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    struct camera *cam = &(scene->cam);
    camera_update(cam);

    //skybox_draw(skybox, cam->view, cam->projection);

    for (uint32_t i = 0; i < scene->point_lights->len; i++) {
        struct point_light *light = darray_at(scene->point_lights, i);
        if (!light->visible) continue;

        struct model light_model = light->geometric_model;
        struct shader shader = light_model.material.shader;
        shader_use(shader);
        model_translate(&light_model, light->pos);
        shader_uniform_vec3(shader, "u_color", light->color);
        shader_uniform_mat4(shader, "u_view", cam->view);
        shader_uniform_mat4(shader, "u_projection", cam->projection);
        model_draw(light_model, shader);
    }

    for (uint32_t i = 0; i < scene->models->len; i++) {
        struct model *model = darray_at(scene->models, i);
        if (!model->visible) continue;

        struct material material = model->material;
        struct shader shader = material.shader;

        /* pre-rendering model */
        shader_use(shader);
        shader_uniform_mat4(shader, "u_view", cam->view);
        shader_uniform_vec3(shader, "u_view_pos", cam->pos);
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
