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

#include "camera.h"
#include "lighting.h"
#include "darray.h"
#include "skybox.h"

/* A good definition of what a scene is can be taken from the website: 
   scratch a pixel
 
        "...However, a 3D scene encompasses more than just geometry. While
        geometry is a critical element, a camera is also needed to view the
        scene. Thus, a scene description typically includes a camera. Moreover,
        a scene without light would appear black, necessitating the inclusion of
        lights. In rendering, all this information (the geometry, camera, and
        lights) is contained within a file called the scene file."

   And that's pretty much what a scene is, given its theatrical nature, it can
   also be thought of at the actual "stage" of a play, with all the actors &
   props representing the geometry (or what we can call as models), the lighting
   so that we can see the objects, and of course the audience (or the camera).

   There are also some terms such as a Scene Graph which is a data structure
   used for representing scenes in a tree-like hierarchy, We recommend the
   reader to take some time reading more about it, it's very interesting and
   also because the scene representation of this project is just a flat struct
   of arrays.

   With that in mind, the project's scene is a struct that contains; a camera;
   a directional light (an example, the sun) and point lights (they are
   most similar to light bulbs); and models, which are composed of geometry and
   attributes for how its supposed to look like when rendered

   The scene is just the actual "visual" aspect of the project! */
struct scene {
    struct camera cam; 
    struct directional_light environment_light;
    struct skybox skybox;
    /* darrays are just dynamically-allocated arrays that can resize itself
       when it gets full, thus it differs from a normal array in the fact that
       it's located on the heap. We use it for models & point lights because
       it's easier to manage however many objets a person might want to load,
       and it ideally saves space compared to using a statically-allocated array
       where parts of its memory might not be used */
    darray *models;
    darray *point_lights;
    bool draw_skybox;
    vec3 clear_color;
};

/* There are only 3 scene functons used and that is for initialization,
   rendering the scene, and after, destroying the scene once the program ends.
    
   For initialization, we pass the viewport dimensions because that's where the
   size of oour visible area will be at, more specifically, it'll give the
   camera its aspect ratio and is primarily used for setting up the camera.
   Besides that, initializing the scene is where the 
   */
void scene_init(struct scene *scene, float viewport_width, float viewport_height);
void scene_render(struct scene *scene);
void scene_destroy(struct scene *scene);

#endif /* SAGE_SCENE_H */
