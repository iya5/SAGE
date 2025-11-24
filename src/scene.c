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
#include "mnf/mnf_util.h"

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

    scene->draw_skybox = true;

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
    model_scale(&sphere, (vec3){0.2, 0.2, 0.2});
    model_translate(&sphere, (vec3){1.40, 4.70, -2.55});
    model_set_name(&sphere, "Sphere");
    darray_push(scene->models, &sphere);
    
    struct model avocado = model_load_from_file("res/avocado.obj");
    avocado.material = material_create("res/avocado/textures/avocado_albedo.jpeg", 
                                       "res/avocado/textures/avocado_specular.jpeg", 1);
    avocado.material.shader = phong_shader;
    model_scale(&avocado, (vec3){6, 6, 6});
    model_rotation(&avocado, (vec3){MNF_RAD(-46), MNF_RAD(6), MNF_RAD(-133)});
    model_translate(&avocado, (vec3){0.10, 4.78, 1.20});
    model_set_name(&avocado, "Avocado");
    darray_push(scene->models, &avocado);
        
    struct model croissant = model_load_from_file("res/croissant.obj");
    croissant.material = material_create("res/croissant/textures/croissant_albedo.jpeg", 
                                         "res/croissant/textures/croissant_albedo.jpeg", 1);
    croissant.material.shader = phong_shader;
    model_scale(&croissant, (vec3){6, 6, 6});
    model_rotation(&croissant, (vec3){MNF_RAD(0), MNF_RAD(-105), MNF_RAD(0)});
    model_translate(&croissant, (vec3){0.05, 4.34, -1.09});
    model_set_name(&croissant, "Croissant");
    darray_push(scene->models, &croissant);
            
    struct model lemon = model_load_from_file("res/lemon.obj");
    lemon.material = material_create("res/lemon/textures/lemon_albedo.jpeg", 
                                     "res/lemon/textures/lemon_specular.jpeg", 1);
    lemon.material.shader = phong_shader;
    model_scale(&lemon, (vec3){8, 8, 8});
    model_rotation(&lemon, (vec3){MNF_RAD(67), MNF_RAD(38), MNF_RAD(96)});
    model_translate(&lemon, (vec3){-0.10, 4.82, 1.28});
    model_set_name(&lemon, "Lemon");
    darray_push(scene->models, &lemon);
        
    struct model lime = model_load_from_file("res/lime.obj");
    lime.material = material_create("res/lime/textures/lime_albedo.jpeg", 
                                    "res/lime/textures/lime_specular.jpeg", 1);
    lime.material.shader = phong_shader;
    model_scale(&lime, (vec3){5, 5, 5});
    model_rotation(&lime, (vec3){MNF_RAD(67), MNF_RAD(80), MNF_RAD(-25)});
    model_translate(&lime, (vec3){-0.43, 4.67, 1.25});
    model_set_name(&lime, "Lime");
    darray_push(scene->models, &lime);
        
    struct model orange = model_load_from_file("res/orange.obj");
    orange.material = material_create("res/orange/textures/orange_albedo.jpeg", 
                                      "res/orange/textures/orange_specular.jpeg", 1);
    orange.material.shader = phong_shader;
    model_scale(&orange, (vec3){5, 5, 5});
    model_rotation(&orange, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_translate(&orange, (vec3){-0.30, 4.44, 1.03});
    model_set_name(&orange, "Orange");
    darray_push(scene->models, &orange); 
    
    struct model bowl = model_load_from_file("res/bowl.obj");
    bowl.material = material_create("res/bowl/textures/bowl.jpeg", 
                                    "res/bowl/textures/bowl_occlusion.jpeg", 1);
    bowl.material.shader = phong_shader;
    model_scale(&bowl, (vec3){5.1, 5.1, 5.1});
    model_rotation(&bowl, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_translate(&bowl, (vec3){0.36, 4.30, 0.60});
    model_set_name(&bowl, "Bowl");
    darray_push(scene->models, &bowl);
        
    struct model cucumber = model_load_from_file("res/cucumber.obj");
    cucumber.material = material_create("res/cucumber/textures/cucumber.jpeg", 
                                        NULL, 1);
    cucumber.material.shader = phong_shader;
    model_scale(&cucumber, (vec3){6, 6, 6});
    model_rotation(&cucumber, (vec3){MNF_RAD(239), MNF_RAD(42), MNF_RAD(85)});
    model_translate(&cucumber, (vec3){0, 4.84, 1.74});
    model_set_name(&cucumber, "Cucumber");
    darray_push(scene->models, &cucumber);
    
    struct model peach = model_load_from_file("res/peach.obj");
    peach.material = material_create("res/peach/textures/peach.jpg", 
                                     NULL, 1);
    peach.material.shader = phong_shader;
    model_scale(&peach, (vec3){0.8, 0.8, 0.8});
    model_rotation(&peach, (vec3){MNF_RAD(72), MNF_RAD(-161), MNF_RAD(21)});
    model_translate(&peach, (vec3){-0.04, 4.74, 0.79});
    model_set_name(&peach, "Peach");
    darray_push(scene->models, &peach);
    
    struct model cake = model_load_from_file("res/cake.obj");
    cake.material = material_create("res/cake/textures/cake.jpeg", 
                                    NULL, 1);
    cake.material.shader = phong_shader;
    model_translate(&cake, (vec3){1.29, 4.34, -0.44});
    model_rotation(&cake, (vec3){MNF_RAD(0), MNF_RAD(-4), MNF_RAD(0)});
    model_scale(&cake, (vec3){6, 6, 6});
    model_set_name(&cake, "Cake");
    darray_push(scene->models, &cake);
    
    struct model pudding = model_load_from_file("res/pudding.obj");
    pudding.material = material_create("res/pudding/textures/pudding.jpeg", 
                                    NULL, 1);
    pudding.material.shader = phong_shader;
    model_translate(&pudding, (vec3){-0.84, 4.35, 0.60});
    model_rotation(&pudding, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&pudding, (vec3){6, 6, 6});
    model_set_name(&pudding, "Pudding");
    darray_push(scene->models, &pudding);

    struct model homemade = model_load_from_file("res/homemade-bread.obj");
    homemade.material = material_create("res/homemade/textures/homemade.jpeg", 
                                    NULL, 1);
    homemade.material.shader = phong_shader;
    model_translate(&homemade, (vec3){0.28, 4.30, -1.43});
    model_rotation(&homemade, (vec3){MNF_RAD(0), MNF_RAD(89), MNF_RAD(-0.99)});
    model_scale(&homemade, (vec3){4, 4, 4});
    model_set_name(&homemade, "Homemade bread");
    darray_push(scene->models, &homemade);
    
    struct model melon_bread = model_load_from_file("res/melon-bread.obj");
    melon_bread.material = material_create("res/melon_bread/textures/melon-bread.jpeg", 
                                    NULL, 1);
    melon_bread.material.shader = phong_shader;
    model_translate(&melon_bread, (vec3){-0.44, 4.34, -1.04});
    model_rotation(&melon_bread, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&melon_bread, (vec3){4, 4, 4});
    model_set_name(&melon_bread, "Melon bread");
    darray_push(scene->models, &melon_bread);

    struct model wooden_tray = model_load_from_file("res/wooden-tray.obj");
    wooden_tray.material = material_create("res/wooden_tray/textures/tray.jpg", 
                                            "res/wooden_tray/textures/tray_ROUGHNESS.jpg", 1);
    wooden_tray.material.shader = phong_shader;
    model_translate(&wooden_tray, (vec3){-0.39, 4.13, -2.08});
    model_rotation(&wooden_tray, (vec3){MNF_RAD(0), MNF_RAD(-86), MNF_RAD(0)});
    model_scale(&wooden_tray, (vec3){6, 6, 6});
    model_set_name(&wooden_tray, "Wooden tray");
    darray_push(scene->models, &wooden_tray);
    
    struct model cake_plate = model_load_from_file("res/dinner-plate.obj");
    cake_plate.material = material_create("res/dinner_plate/textures/dinner_plate.jpeg", 
                                            NULL, 1);
    cake_plate.material.shader = phong_shader;
    model_translate(&cake_plate, (vec3){0.95, 4.19, 0.24});
    model_rotation(&cake_plate, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&cake_plate, (vec3){7, 7, 7});
    model_set_name(&cake_plate, "Cake plate");
    darray_push(scene->models, &cake_plate);
    
    struct model pudding_plate = model_load_from_file("res/dinner-plate.obj");
    pudding_plate.material = material_create("res/dinner_plate/textures/dinner_plate.jpeg", 
                                            NULL, 1);
    pudding_plate.material.shader = phong_shader;
    model_translate(&pudding_plate, (vec3){-0.29, 4.19, 0.05});
    model_rotation(&pudding_plate, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&pudding_plate, (vec3){6, 6, 6});
    model_set_name(&pudding_plate, "Pudding plate");
    darray_push(scene->models, &pudding_plate);

    /*
    struct model bread_seeds = model_load_from_file("res/bread-seeds.obj");
    bread_seeds.material = material_create("res/bread-seeds/textures/bread-seeds.jpeg", 
                                    NULL, 1);
    bread_seeds.material.shader = phong_shader;
    model_translate(&bread_seeds, (vec3){0.21, 5.64, -0.13});
    model_rotation(&bread_seeds, (vec3){MNF_RAD(40), MNF_RAD(16.99), MNF_RAD(-7)});
    model_scale(&bread_seeds, (vec3){0.05, 0.05, 0.05});
    model_set_name(&bread_seeds, "Bread with seeds");
    darray_push(scene->models, &bread_seeds);
    */


    struct model wooden_table = model_load_from_file("res/wooden-table.obj");
    wooden_table.material = material_create("res/wooden_table/textures/table.png", 
                                            NULL, 1);
    wooden_table.material.shader = phong_shader;
    model_scale(&wooden_table, (vec3){7, 7, 7});
    model_rotation(&wooden_table, (vec3){MNF_RAD(0), MNF_RAD(90), MNF_RAD(0)});
    model_translate(&wooden_table, (vec3){0, -0.2, 0});
    model_set_name(&wooden_table, "Wooden table");
    darray_push(scene->models, &wooden_table);
    
    struct model wooden_chair = model_load_from_file("res/wooden-chair.obj");
    wooden_chair.material = material_create("res/wooden_chair/textures/chair.png", 
                                            NULL, 1);
    wooden_chair.material.shader = phong_shader;
    model_scale(&wooden_chair, (vec3){6, 6, 6});
    model_rotation(&wooden_chair, (vec3){MNF_RAD(0), MNF_RAD(90), MNF_RAD(0)});
    model_translate(&wooden_chair, (vec3){1.7, -0.2, -1.45});
    model_set_name(&wooden_chair, "Wooden chair");
    darray_push(scene->models, &wooden_chair);
    
    
    struct model light_body = model_load_from_file("res/sphere.obj");
    light_body.material = material_create(NULL, NULL, 1);
    light_body.material.shader = light_shader;
    model_scale(&light_body, (vec3){0.1, 0.1, 0.1});

    struct model floor = model_create_cube();
    floor.material = material_create("res/textures/base.png", NULL, 1);
    floor.material.shader = phong_shader;
    model_scale(&floor, (vec3){50, 0.1, 50});
    model_rotation(&floor, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_translate(&floor, (vec3){0, -0.24, 0});
    model_set_name(&floor, "Floor");
    darray_push(scene->models, &floor);

    /* Creating light */
    struct directional_light environment_light = {
        .direction = {1, -1.0, 1.5},
        .ambient = {100.0/255.0, 100.0/255.0, 100.0/255.0},
        .diffuse = {150.0/255.0, 150.0/255.0, 150.0/255.0},
        .specular = {50.0/255.0, 50.0/255.0, 50.0/255.0}
    };
    scene->environment_light = environment_light;

    struct point_light r_light = {
        .color = {1.0, 0.0, 0.0},
        .pos = {0.60, 9.0, 4.0},
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
        .pos = {0.60, 9.0, 0},
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
        .pos = {0.60, 9.0, -4},
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

    if (scene->draw_skybox) skybox_draw(skybox, cam->view, cam->projection);

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
