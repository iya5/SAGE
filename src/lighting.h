#ifndef SAGE_LIGHTING_H
#define SAGE_LIGHTING_H

#include "mnf/mnf_types.h"
#include "darray.h"
#include "shader.h"
#include "model.h"

#define LIGHT_NAME_MAX_SIZE 100

struct directional_light {
    /* light source to object */
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct point_light {
    vec3 color;
    vec3 pos;
    vec3 diffuse;
    vec3 specular;
    char name[LIGHT_NAME_MAX_SIZE];
    float attenuation_range;

    float constant;
    float linear;
    float quadratic;

    /* if this is NULL, the light isn't geometrically rendered but still exists
       in the scene */
    struct model geometric_model;
    bool visible;
};

struct lighting_params {
    bool enable_ambient;
    bool enable_diffuse;
    bool enable_specular;
};

struct point_light point_light_create(const char *name, float attenuation_range);
void point_light_set_attenuation_range(struct point_light *light, float range);

void point_light_set_pos(struct point_light *light, vec3 pos);
void point_light_set_color(struct point_light *light, vec3 color);
void point_light_set_diffuse(struct point_light *light, vec3 diffuse);
void point_light_set_specular(struct point_light *light, vec3 specular);

void lighting_apply(struct shader active_shader,
                    struct directional_light directional_light,
                    darray *point_lights,
                    struct lighting_params params);

void light_set_name(struct point_light *light, const char name[LIGHT_NAME_MAX_SIZE]);


#endif /* SAGE_LIGHTING_H */
