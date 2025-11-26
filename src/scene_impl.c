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

/* Scene files are just plain C files which run based on the macro defined below
   but only one scene can be initialized as any given time (it's possible but
   it simply combines them all together which is probably not the expected
   result */

#include "scene.h"
#include "mnf/mnf_util.h"

#define MINECRAFT

#ifdef SCENE_ONE

void scene_init_models(struct scene *scene)
{
    
    struct model jake = model_load_from_file("res/jake-the-dog.obj");
    jake.material = material_create("res/jake/textures/jake.png", NULL, 80);
    model_translate(&jake, (vec3){0, 0, 0.5});
    model_rotation(&jake, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&jake, (vec3){5, 5, 5});
    model_set_name(&jake, "Jake the Dog");
    darray_push(scene->models, &jake);

    /*
    struct model floor = model_create_cube();
    floor.material = material_create("res/textures/base.png", NULL, 1);
    model_scale(&floor, (vec3){30, 0.1, 30});
    model_translate(&floor, (vec3){0, -2, 0});
    model_set_name(&floor, "Floor");
    darray_push(scene->models, &floor);
    */
}

void scene_init_lighting(struct scene *scene)
{
    struct model light_body = model_load_from_file("res/sphere.obj");
    light_body.material = material_create(NULL, NULL, 1);
    model_scale(&light_body, (vec3){0.1, 0.1, 0.1});

    struct directional_light environment_light = {
        .direction = {0, -1.0, 0},
        .ambient = {0.3, 0.3, 0.3},
        .diffuse = {0.5, 0.5, 0.5},
        .specular = {0.2, 0.2, 0.2}
    };
    scene->environment_light = environment_light;

    struct point_light r_light = {
        .color = {1.0, 0.0, 0.0},
        .pos = {1.45, 4.0, 0.0},
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
        .pos = {-1.45, 4.0, 2.1},
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
        .pos = {-1.45, 4.0, -2.1},
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
}

void scene_init_skybox(struct scene *scene)
{
    const char *cubemap_faces[6] = {
        "res/skybox/default/px.jpg",
        "res/skybox/default/nx.jpg",
        "res/skybox/default/py.jpg",
        "res/skybox/default/ny.jpg",
        "res/skybox/default/pz.jpg",
        "res/skybox/default/nz.jpg",
    };
    skybox_init(&scene->skybox, cubemap_faces);
}

#endif


#ifdef SCENE_TWO

void scene_init_models(struct scene *scene)
{
    struct model avocado = model_load_from_file("res/avocado.obj");
    avocado.material = material_create("res/avocado/textures/avocado_albedo.jpeg", 
                                       "res/avocado/textures/avocado_specular.jpeg", 1);
    model_scale(&avocado, (vec3){6, 6, 6});
    model_rotation(&avocado, (vec3){MNF_RAD(-46), MNF_RAD(6), MNF_RAD(-133)});
    model_translate(&avocado, (vec3){0.10, 4.78, 1.20});
    model_set_name(&avocado, "Avocado");
    darray_push(scene->models, &avocado);
        
    struct model croissant = model_load_from_file("res/croissant.obj");
    croissant.material = material_create("res/croissant/textures/croissant_albedo.jpeg", 
                                         "res/croissant/textures/croissant_albedo.jpeg", 1);
    model_scale(&croissant, (vec3){6, 6, 6});
    model_rotation(&croissant, (vec3){MNF_RAD(0), MNF_RAD(-105), MNF_RAD(0)});
    model_translate(&croissant, (vec3){0.05, 4.34, -1.09});
    model_set_name(&croissant, "Croissant");
    darray_push(scene->models, &croissant);
            
    struct model lemon = model_load_from_file("res/lemon.obj");
    lemon.material = material_create("res/lemon/textures/lemon_albedo.jpeg", 
                                     "res/lemon/textures/lemon_specular.jpeg", 1);
    model_scale(&lemon, (vec3){8, 8, 8});
    model_rotation(&lemon, (vec3){MNF_RAD(67), MNF_RAD(38), MNF_RAD(96)});
    model_translate(&lemon, (vec3){-0.10, 4.82, 1.28});
    model_set_name(&lemon, "Lemon");
    darray_push(scene->models, &lemon);
        
    struct model lime = model_load_from_file("res/lime.obj");
    lime.material = material_create("res/lime/textures/lime_albedo.jpeg", 
                                    "res/lime/textures/lime_specular.jpeg", 1);
    model_scale(&lime, (vec3){5, 5, 5});
    model_rotation(&lime, (vec3){MNF_RAD(67), MNF_RAD(80), MNF_RAD(-25)});
    model_translate(&lime, (vec3){-0.43, 4.67, 1.25});
    model_set_name(&lime, "Lime");
    darray_push(scene->models, &lime);
        
    struct model orange = model_load_from_file("res/orange.obj");
    orange.material = material_create("res/orange/textures/orange_albedo.jpeg", 
                                      "res/orange/textures/orange_specular.jpeg", 1);
    model_scale(&orange, (vec3){5, 5, 5});
    model_rotation(&orange, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_translate(&orange, (vec3){-0.30, 4.44, 1.03});
    model_set_name(&orange, "Orange");
    darray_push(scene->models, &orange);

    struct model bowl = model_load_from_file("res/bowl.obj");
    bowl.material = material_create("res/bowl/textures/bowl.jpeg", 
                                    "res/bowl/textures/bowl_occlusion.jpeg", 1);
    model_scale(&bowl, (vec3){5.1, 5.1, 5.1});
    model_rotation(&bowl, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_translate(&bowl, (vec3){0.36, 4.30, 0.60});
    model_set_name(&bowl, "Bowl");
    darray_push(scene->models, &bowl);
        
    struct model cucumber = model_load_from_file("res/cucumber.obj");
    cucumber.material = material_create("res/cucumber/textures/cucumber.jpeg", 
                                        NULL, 1);
    model_scale(&cucumber, (vec3){6, 6, 6});
    model_rotation(&cucumber, (vec3){MNF_RAD(239), MNF_RAD(42), MNF_RAD(85)});
    model_translate(&cucumber, (vec3){0, 4.84, 1.74});
    model_set_name(&cucumber, "Cucumber");
    darray_push(scene->models, &cucumber);
    
    struct model peach = model_load_from_file("res/peach.obj");
    peach.material = material_create("res/peach/textures/peach.jpg", 
                                     NULL, 1);
    model_translate(&peach, (vec3){-0.04, 4.64, 0.89});
    model_rotation(&peach, (vec3){MNF_RAD(58), MNF_RAD(154.01), MNF_RAD(-19)});
    model_scale(&peach, (vec3){4, 4, 4});
    model_set_name(&peach, "Peach");
    darray_push(scene->models, &peach);
    
    struct model cake = model_load_from_file("res/cake.obj");
    cake.material = material_create("res/cake/textures/cake.jpeg", 
                                    NULL, 1);
    model_translate(&cake, (vec3){1.29, 4.34, -0.44});
    model_rotation(&cake, (vec3){MNF_RAD(0), MNF_RAD(-4), MNF_RAD(0)});
    model_scale(&cake, (vec3){6, 6, 6});
    model_set_name(&cake, "Cake");
    darray_push(scene->models, &cake);
    
    struct model pudding = model_load_from_file("res/pudding.obj");
    pudding.material = material_create("res/pudding/textures/pudding.jpeg", 
                                    NULL, 1);
    model_translate(&pudding, (vec3){-0.84, 4.35, 0.60});
    model_rotation(&pudding, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&pudding, (vec3){6, 6, 6});
    model_set_name(&pudding, "Pudding");
    darray_push(scene->models, &pudding);

    struct model homemade = model_load_from_file("res/homemade-bread.obj");
    homemade.material = material_create("res/homemade/textures/homemade.jpeg", 
                                    NULL, 1);
    model_translate(&homemade, (vec3){0.28, 4.30, -1.43});
    model_rotation(&homemade, (vec3){MNF_RAD(0), MNF_RAD(89), MNF_RAD(-0.99)});
    model_scale(&homemade, (vec3){4, 4, 4});
    model_set_name(&homemade, "Homemade bread");
    darray_push(scene->models, &homemade);
    
    struct model melon_bread = model_load_from_file("res/melon-bread.obj");
    melon_bread.material = material_create("res/melon_bread/textures/melon-bread.jpeg", 
                                    NULL, 1);
    model_translate(&melon_bread, (vec3){-0.44, 4.34, -1.04});
    model_rotation(&melon_bread, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&melon_bread, (vec3){4, 4, 4});
    model_set_name(&melon_bread, "Melon bread");
    darray_push(scene->models, &melon_bread);

    struct model wooden_tray = model_load_from_file("res/wooden-tray.obj");
    wooden_tray.material = material_create("res/wooden_tray/textures/tray.jpg", 
                                            "res/wooden_tray/textures/tray_ROUGHNESS.jpg", 1);
    model_translate(&wooden_tray, (vec3){-0.39, 4.13, -2.08});
    model_rotation(&wooden_tray, (vec3){MNF_RAD(0), MNF_RAD(-86), MNF_RAD(0)});
    model_scale(&wooden_tray, (vec3){6, 6, 6});
    model_set_name(&wooden_tray, "Wooden tray");
    darray_push(scene->models, &wooden_tray);
    
    struct model cake_plate = model_load_from_file("res/dinner-plate.obj");
    cake_plate.material = material_create("res/dinner_plate/textures/dinner_plate.jpeg", 
                                            NULL, 1);
    model_translate(&cake_plate, (vec3){0.95, 4.19, 0.24});
    model_rotation(&cake_plate, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&cake_plate, (vec3){7, 7, 7});
    model_set_name(&cake_plate, "Cake plate");
    darray_push(scene->models, &cake_plate);
    
    struct model pudding_plate = model_load_from_file("res/dinner-plate.obj");
    pudding_plate.material = material_create("res/dinner_plate/textures/dinner_plate.jpeg", 
                                            NULL, 1);
    model_translate(&pudding_plate, (vec3){-0.29, 4.19, 0.05});
    model_rotation(&pudding_plate, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&pudding_plate, (vec3){6, 6, 6});
    model_set_name(&pudding_plate, "Pudding plate");
    darray_push(scene->models, &pudding_plate);
    
    struct model v_coffee = model_load_from_file("res/vienna.obj");
    v_coffee.material = material_create("res/vienna-coffee/textures/vienna.jpeg", 
                                        "res/vienna-coffee/textures/vienna_ao.jpeg", 1);
    model_translate(&v_coffee, (vec3){-0.20, 4.34, -1.66});
    model_rotation(&v_coffee, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&v_coffee, (vec3){5, 5, 5});
    model_set_name(&v_coffee, "Vienna Coffee");
    darray_push(scene->models, &v_coffee);
    
    struct model coffee = model_load_from_file("res/coffee.obj");
    coffee.material = material_create("res/coffee/textures/coffee_alb.jpg", 
                                        "res/coffee/textures/coffee_sp.jpg", 1);
    model_translate(&coffee, (vec3){-0.76, 4.34, -1.66});
    model_rotation(&coffee, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&coffee, (vec3){22, 22, 22});
    model_set_name(&coffee, "Coffee");
    darray_push(scene->models, &coffee);
    
    struct model pot = model_load_from_file("res/teapot.obj");
    pot.material = material_create("res/coffee-pot/textures/teapot_alb.png", 
                                        "res/coffee-pot/textures/teapot_roughness.png", 1);
    model_translate(&pot, (vec3){-0.07, 4.33, -1.96});
    model_rotation(&pot, (vec3){MNF_RAD(0), MNF_RAD(124), MNF_RAD(0)});
    model_scale(&pot, (vec3){25, 25, 25});
    model_set_name(&pot, "Coffee pot");
    darray_push(scene->models, &pot);

    struct model slv = model_load_from_file("res/silverware.obj");
    slv.material = material_create(NULL, NULL , 2048);
    model_translate(&slv, (vec3){-0.93, 4.05, -1.04});
    model_rotation(&slv, (vec3){MNF_RAD(5.6), MNF_RAD(47), MNF_RAD(-7.90)});
    model_scale(&slv, (vec3){30, 30, 30});
    model_set_name(&slv, "Silverware");
    darray_push(scene->models, &slv);
    
    struct model tissue = model_load_from_file("res/tissue.obj");

    tissue.material = material_create("res/tissue/textures/tissue_alb.png", 
                                    NULL, 1);
    model_translate(&tissue, (vec3){0.52, 4.26, -1.91});
    model_rotation(&tissue, (vec3){MNF_RAD(93), MNF_RAD(185), MNF_RAD(2.20)});
    model_scale(&tissue, (vec3){25, 45, 10});
    model_set_name(&tissue, "Tissue");
    darray_push(scene->models, &tissue);
    
    struct model tissue2 = model_load_from_file("res/tissue.obj");

    tissue2.material = material_create("res/tissue/textures/tissue_alb.png", 
                                    NULL, 1);
    model_translate(&tissue2, (vec3){0.59, 4.22, -1.90});
    model_rotation(&tissue2, (vec3){MNF_RAD(90), MNF_RAD(182.6), MNF_RAD(0)});
    model_scale(&tissue2, (vec3){25, 45, 10});
    model_set_name(&tissue2, "Tissue 2");
    darray_push(scene->models, &tissue2);
    
    
    struct model socrates = model_load_from_file("res/socrates.obj");

    socrates.material = material_create("res/socrates/textures/soc_alb.png", 
                                        "res/socrates/textures/soc_rough.png", 1);
    model_translate(&socrates, (vec3){1.19, 4.3, 1.66});
    model_rotation(&socrates, (vec3){MNF_RAD(0), MNF_RAD(25.9), MNF_RAD(0)});
    model_scale(&socrates, (vec3){4, 4, 4});
    model_set_name(&socrates, "Socrates");
    darray_push(scene->models, &socrates);  

    struct model wooden_table = model_load_from_file("res/wooden-table.obj");
    wooden_table.material = material_create("res/wooden_table/textures/table.png", 
                                            NULL, 1);
    model_scale(&wooden_table, (vec3){7, 7, 7});
    model_rotation(&wooden_table, (vec3){MNF_RAD(0), MNF_RAD(90), MNF_RAD(0)});
    model_translate(&wooden_table, (vec3){0, -0.2, 0});
    model_set_name(&wooden_table, "Wooden table");
    darray_push(scene->models, &wooden_table);
    
    struct model wooden_chair = model_load_from_file("res/wooden-chair.obj");
    wooden_chair.material = material_create("res/wooden_chair/textures/chair.png", 
                                            NULL, 1);
    model_scale(&wooden_chair, (vec3){6, 6, 6});
    model_rotation(&wooden_chair, (vec3){MNF_RAD(0), MNF_RAD(90), MNF_RAD(0)});
    model_translate(&wooden_chair, (vec3){1.7, -0.2, -1.45});
    model_set_name(&wooden_chair, "Wooden chair");
    darray_push(scene->models, &wooden_chair);
    
    struct model carpet = model_load_from_file("res/carpet.obj");
    carpet.material = material_create("res/carpet/textures/carpet_alb.png", 
                                    "res/carpet/textures/carpet_mtl.png", 1);
    model_translate(&carpet, (vec3){0, -0.22, 0});
    model_rotation(&carpet, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&carpet, (vec3){3, 3, 3});
    model_set_name(&carpet, "Carpet");
    darray_push(scene->models, &carpet);
    
    struct model hw_floor = model_create_cube();
    hw_floor.material = material_create("res/hardwood/textures/hw_diff.jpeg", 
                                        "res/hardwood/textures/hw_spec.png", 1);
    model_translate(&hw_floor, (vec3){0, -0.24, 0});
    model_rotation(&hw_floor, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&hw_floor, (vec3){40, 0.1, 40});
    model_set_name(&hw_floor, "Floor");
    darray_push(scene->models, &hw_floor);
}

void scene_init_lighting(struct scene *scene)
{
    struct model light_body = model_load_from_file("res/sphere.obj");
    light_body.material = material_create(NULL, NULL, 1);
    model_scale(&light_body, (vec3){0.1, 0.1, 0.1});

    struct directional_light environment_light = {
        .direction = {1, -1.0, 1.5},
        .ambient = {100.0/255.0, 100.0/255.0, 100.0/255.0},
        .diffuse = {150.0/255.0, 150.0/255.0, 150.0/255.0},
        .specular = {50.0/255.0, 50.0/255.0, 50.0/255.0}
    };
    scene->environment_light = environment_light;

    struct point_light plight_1 = point_light_create("Point Light 1", 50.0);
    point_light_set_color(&plight_1, (vec3){1.0, 1.0, 1.0});
    point_light_set_pos(&plight_1, (vec3){0.60, 9.0, 4.0});
    point_light_set_diffuse(&plight_1, (vec3){1.0, 1.0, 1.0});
    point_light_set_specular(&plight_1, (vec3){1.0, 1.0, 1.0});
    plight_1.geometric_model = light_body;
    darray_push(scene->point_lights, &plight_1);
}

void scene_init_skybox(struct scene *scene)
{
    const char *cubemap_faces[6] = {
        "res/skybox/default/px.jpg",
        "res/skybox/default/nx.jpg",
        "res/skybox/default/py.jpg",
        "res/skybox/default/ny.jpg",
        "res/skybox/default/pz.jpg",
        "res/skybox/default/nz.jpg",
    };
    skybox_init(&scene->skybox, cubemap_faces);
}

#endif


#ifdef SCENE_THREE

void scene_init_models(struct scene *scene)
{
    struct model socrates = model_load_from_file("res/socrates.obj");

    socrates.material = material_create("res/socrates/textures/soc_alb.png", 
                                    "res/socrates/textures/soc_rough.png", 1);
    model_translate(&socrates, (vec3){0, 0, 0});
    model_rotation(&socrates, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&socrates, (vec3){6, 6, 6});
    model_set_name(&socrates, "Socrates");
    darray_push(scene->models, &socrates);
}

void scene_init_lighting(struct scene *scene)
{
    struct model light_body = model_load_from_file("res/sphere.obj");
    light_body.material = material_create(NULL, NULL, 1);
    model_scale(&light_body, (vec3){0.1, 0.1, 0.1});

    struct directional_light environment_light = {
        .direction = {1, -1.0, 1.5},
        .ambient = {60.0/255.0, 100.0/255.0, 100.0/255.0},
        .diffuse = {150.0/255.0, 150.0/255.0, 150.0/255.0},
        .specular = {50.0/255.0, 50.0/255.0, 50.0/255.0}
    };
    scene->environment_light = environment_light;

    struct point_light plight_1 = point_light_create("Point Light 1", 50.0);
    point_light_set_color(&plight_1, (vec3){1.0, 1.0, 1.0});
    point_light_set_pos(&plight_1, (vec3){-4.95, 4.44, 1.29});
    point_light_set_diffuse(&plight_1, (vec3){1.0, 1.0, 1.0});
    point_light_set_specular(&plight_1, (vec3){1.0, 1.0, 1.0});
    plight_1.geometric_model = light_body;
    darray_push(scene->point_lights, &plight_1);
}

void scene_init_skybox(struct scene *scene)
{
    const char *cubemap_faces[6] = {
        "res/skybox/default/px.jpg",
        "res/skybox/default/nx.jpg",
        "res/skybox/default/py.jpg",
        "res/skybox/default/ny.jpg",
        "res/skybox/default/pz.jpg",
        "res/skybox/default/nz.jpg",
    };
    skybox_init(&scene->skybox, cubemap_faces);
}

#endif

#ifdef MINECRAFT

void scene_init_models(struct scene *scene)
{
    struct model mc_castle = model_load_from_file("res/castle.obj");
    mc_castle.material = material_create("res/minecraft-castle/textures/castle.jpg", NULL, 80);
    model_translate(&mc_castle, (vec3){0, 0, 0});
    model_rotation(&mc_castle, (vec3){MNF_RAD(0), MNF_RAD(0), MNF_RAD(0)});
    model_scale(&mc_castle, (vec3){16, 16, 16});
    model_set_name(&mc_castle, "Minecraft Castle");
    darray_push(scene->models, &mc_castle);
}

void scene_init_lighting(struct scene *scene)
{
    struct model light_body = model_load_from_file("res/sphere.obj");
    light_body.material = material_create(NULL, NULL, 1);
    model_scale(&light_body, (vec3){0.1, 0.1, 0.1});

    struct directional_light environment_light = {
        .direction = {16.04, -1.04, 2.95},
        .ambient = {150.0/255.0, 150.0/255.0, 150.0/255.0},
        .diffuse = {200.0/255.0, 200.0/255.0, 200.0/255.0},
        .specular = {50.0/255.0, 50.0/255.0, 50.0/255.0}
    };
    scene->environment_light = environment_light;

    struct point_light plight_1 = point_light_create("Point Light 1", 50.0);
    point_light_set_color(&plight_1, (vec3){1.0, 1.0, 1.0});
    point_light_set_pos(&plight_1, (vec3){37, 41.99, 10.44});
    point_light_set_diffuse(&plight_1, (vec3){1.0, 1.0, 1.0});
    point_light_set_specular(&plight_1, (vec3){1.0, 1.0, 1.0});
    plight_1.geometric_model = light_body;
    darray_push(scene->point_lights, &plight_1);
}

void scene_init_skybox(struct scene *scene)
{
    const char *cubemap_faces[6] = {
        "res/skybox/default/px.jpg",
        "res/skybox/default/nx.jpg",
        "res/skybox/default/py.jpg",
        "res/skybox/default/ny.jpg",
        "res/skybox/default/pz.jpg",
        "res/skybox/default/nz.jpg",
    };
    skybox_init(&scene->skybox, cubemap_faces);
}

#endif
