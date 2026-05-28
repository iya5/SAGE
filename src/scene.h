#ifndef SAGE_SCENE_H
#define SAGE_SCENE_H

#include "camera.h"
#include "lighting.h"
#include "darray.h"
#include "skybox.h"

struct scene {
    struct camera cam; 
    struct directional_light environment_light;
    struct skybox skybox;
    darray *models;
    darray *point_lights;
    vec3 clear_color;

    bool draw_skybox;
    struct lighting_params lighting_params;
};

void scene_init(struct scene *scene, float viewport_width, float viewport_height);
void scene_render(struct scene *scene);
void scene_destroy(struct scene *scene);

void scene_init_models(struct scene *scene);
void scene_init_lighting(struct scene *scene);
void scene_init_skybox(struct scene *scene);

#endif /* SAGE_SCENE_H */
