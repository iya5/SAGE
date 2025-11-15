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

#include "shader.h"
#include "texture.h"


/* https://alelievr.github.io/Modern-Rendering-Introduction/Material/
   In real-time computer graphics, a Material is an object that contains the
   various properties needed to render a surface. These properties can include
   textures, colors, numbers, etc. They usually map directly to the data used in
   shaders to render the objects, which is why most materials also reference a
   shader capable of reading the material data. Most 3D engines now provide the
   ability to create new shaders and use materials to store values for these
   shaders to render a particular object. */

struct material {
    struct shader shader;
    struct texture diffuse_map;
    struct texture specular_map;
    float shininess;
};

struct material material_create(const char *diffuse_map,
                                const char *specular_map,
                                float shininess);

struct material material_create_default(void);

#endif /* SAGE_MATERIAL_H */
