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

#include "material.h"
#include "texture.h"
#include "assert.h"

struct material material_create_default(void)
{
    struct material material = {
        .diffuse_map = texture_create_default(),
        .specular_map = texture_create_default(),
        .shininess = 32
    };

    return material;
}

struct material material_create(const char *diffuse_map_path,
                                const char *specular_map_path,
                                float shininess)
{
    /* TODO: figure out whether i create an array of textures in the scene
       so that i don't end up creating more textures & it makes it easier to
       free memory w/o leaking from forgetting to destroy textures in the GPU */
    SASSERT(shininess >= 0);
    struct texture diffuse, specular;

    if (diffuse_map_path == NULL)
        diffuse = texture_create_default();
    else
        diffuse = texture_create(diffuse_map_path);

    if (specular_map_path == NULL)
        specular = texture_create_default();
    else
        specular = texture_create(specular_map_path);

    struct material material = {
        .diffuse_map = diffuse,
        .specular_map = specular,
        .shininess = shininess
    };

    return material;
}

void material_apply(struct material material)
{
    texture_bind(material.diffuse_map, 0);
    texture_bind(material.specular_map, 1);
}
