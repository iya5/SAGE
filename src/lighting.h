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

#ifndef SAGE_LIGHTING_H
#define SAGE_LIGHTING_H

#include "mnf/mnf_types.h"
#include "darray.h"
#include "shader.h"
#include "model.h"

/* Although conceptually the same as a point light, it differs by not having an
   "actual position", and only having a direction, thus every object in a scene
   is shaded by the same direction of this light. It can be thought of as a point
   light whose position is infinitely far away and whose rays illuminate parallel,
   in the same direction everywhere */
struct directional_light {
    /* light source to object */
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/* This point light struct  is simply just a non-geometric light visualizing how
   a light would interact with the scene. It is "non-geometric" because it has
   no geometry attach to, however, it can be displayed by linking a model to it
   through the geometric model pointer */
struct point_light {
    vec3 color;
    vec3 pos;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    /* if this is NULL, the light isn't geometrically rendered but still exists
       in the scene */
    struct model geometric_model;
};

void lighting_apply(struct shader active_shader,
                    struct directional_light directional_light,
                    darray *point_lights);
#endif /* SAGE_LIGHTING_H */
