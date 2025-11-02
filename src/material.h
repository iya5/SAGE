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

#ifndef SAGE_MATERIAL_H
#define SAGE_MATERIAL_H

#include "mnf/mnf_types.h"
#include "shader.h"

/*
 * Materials are just properties of objects and how they react to light. These
 * are principally the ambient, diffuse, specular, and shininess component.
 *
 * From the Unreal Engine Documentation:
 * ... Materials tell the render engine exactly how a surface should interact
 * with the light in your scene. Materials define every aspect of the surface
 * including color, reflectivity, bumpiness, transparency, and so on ...
 *
 * From shapernode
 * Materials are collections of properties that define how a surface interacts
 * with light. They encompass attributes like color, reflectivity, transparency,
 * roughness, and more. Materials are responsible for giving objects their 
 * physical characteristics and visual qualities.
 * ...
 * Material is a set of parameters that dictates how an object looks in a
 * virtual environment. Those parameters can be defined by numeric values and by
 * textures with per-pixel accuracy. For instance, a metallic material would
 * reflect light differently than a rough, matte material.
 */

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

void material_apply(struct material material, struct shader shader);

#endif /* SAGE_MATERIAL_H */
