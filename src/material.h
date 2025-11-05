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
#include "texture.h"

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
 *
 * More Information from stack overflow!
 * https://stackoverflow.com/questions/4262503/whats-the-difference-between-material-and-texture
 *
 * In OpenGL, a material is a set of coefficients that define how the lighting
 * model interacts with the surface. In particular, ambient, diffuse, and
 * specular coefficients for each color component (R,G,B) are defined and
 * applied to a surface and effectively multiplied by the amount of light of
 * each kind/color that strikes the surface. A final emmisivity coefficient is
 * then added to each color component that allows objects to appear luminous
 * without actually interacting with other objects.
 *
 * A texture, on the other hand, is a set of 1-, 2-, 3-, or 4- dimensional
 * bitmap (image) data that is applied and interpolated on to a surface
 * according to texture coordinates at the vertices. Texture data alters the
 * color of the surface whether or not lighting is enabled (and depending on the
 * texture mode, e.g. decal, modulate, etc.). Textures are used frequently to
 * provide sub-polygon level detail to a surface, e.g. applying a repeating
 * brick and mortar texture to a quad to simulate a brick wall, rather than
 * modeling the geometry of each individual brick.
 *
 * In the classical (fixed-pipeline) OpenGL model, textures and materials are
 * somewhat orthogonal. In the new programmable shader world, the line has
 * blurred quite a bit. Frequently textures are used to influence lighting in
 * other ways. For example, bump maps are textures that are used to perturb
 * surface normals to effect lighting, rather than modifying pixel color
 * directly as a regular "image" texture would.
 */

struct material {
    /* Ambient is not included because this is generally the same as diffuse.
     * The reason for textures instead of coefficients is because we can make
     * use of the fragment's colors directly to the lighting model.
     */
    struct texture diffuse;
    struct texture specular;
    float shininess;
};

enum light_type {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT
};

struct light {
    enum light_type type;
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/*
 * Sets coefficient parameters for a given lighting model defined in the shader,
 * for example, a phong shader.
 */
void lighting_model_set_params(struct light light,
                               struct material material,
                               struct shader shader);

#endif /* SAGE_MATERIAL_H */
