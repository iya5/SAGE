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

#ifndef SAGE_SCENE_H
#define SAGE_SCENE_H

/*
 * From scratch a pixel
 * However, a 3D scene encompasses more than just geometry. While geometry is a
 * critical element, a camera is also needed to view the scene. Thus, a scene
 * description typically includes a camera. Moreover, a scene without light
 * would appear black, necessitating the inclusion of lights. In rendering, all
 * this information (the geometry, camera, and lights) is contained within a
 * file called the scene file. The content of the 3D scene can also be loaded
 * into the memory of a 3D package such as Maya or Blender. In such cases, when
 * a user initiates rendering, a special program or plugin processes each object
 * in the scene, along with each light, and exports everything (including the
 * camera) directly to the renderer. Additionally, the renderer requires certain
 * extra information, such as the resolution of the final image, typically
 * referred to as global render settings or options.
 */

#include "camera.h"
#include "material.h"

#define SCENE_MAX_LIGHTS 6
#define SCENE_MAX_MATERIALS 6

struct scene {
    struct light lights[SCENE_MAX_LIGHTS];
    struct material materials[SCENE_MAX_MATERIALS];
    struct camera *cam; 
};

void scene_render(struct scene *scene);

#endif /* SAGE_SCENE_H */
