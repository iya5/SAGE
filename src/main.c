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

#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "platform.h"
#include "ui/ui.h"
#include "scene.h"
#include "input.h"

struct platform platform;
struct ui ui;
struct scene scene;

int main(void)
{
    /* The main() function is the orchestrator and entrance to the application
       where the binary starts running.
     
       All of our initializations occur here, we pass references to our structs
       so that we can declare them statically, plus it's overall nicer.
       
       When the program starts, it begins by starting platform-specific code for
       OpenGL (loading the functions & starting it) as well as input & window
       management. We pass the window's size as well as the viewport (that is,
       the drawable part of the window, typically the same size as the window)
       to our init function.

       The UI is done using Immediate-Mode GUI (drawn every single frame) and
       it too requires an initialization by passing the platform  (to retrieve
       the width & height of the drawable area)

       Finally, the scene is initialized; including the geometry, lighting, &
       camera necessary to view the 3D rendered models.

       When the scene is done, we build the UI's scene graph based on the data
       loaded onto the Scene. There are perhaps better ways of doing this such
       as lazily loading them but for now, this is the implementation.

       All of these initializations will later on be 'deinitialized' for
       gracefully ending the program. */
    platform_window_init(&platform, 
                         SAGE_INITIAL_WINDOW_WIDTH,
                         SAGE_INITIAL_WINDOW_HEIGHT,
                         SAGE_INITIAL_VIEWPORT_WIDTH,
                         SAGE_INITIAL_VIEWPORT_HEIGHT);
    ui_init(&ui, platform);
    scene_init(&scene, platform.viewport_width, platform.viewport_height);
    ui_build_scene_graph(&ui.scene_graph, &scene);

    /* This is our rendering loop (or can also be thought of as the game loop
       which 'runs' the entire program every frame until it closes.

       The general structure of the rendering loop can be thought of as this:
       1. begin the loop
       2. poll for input
       3. process the input
       4. render the UI
       5. render the scene (everything from models, to lighting, to cameras)
       6. swap the back buffer with the front buffer
       7. go backt to the start of the loop & repeat

       There are additional steps such as calculating delta time used for
       the input or other debug options like the FPS (or even better, the time
       per frame!) but generally, it looks like the above, atleast for this
       project.
       */
    while (!platform_should_close(&platform)) {
        platform_update_frame_timing(&platform);
        platform_poll_input(&platform);

        process_input(&scene, &platform);
        ui_begin_frame(&ui, &scene, &platform);
        ui_process_input(&ui, &platform);
        scene_render(&scene);

        ui_end_frame();
        platform_swap_buffer(&platform);
    }

    /* After the program proceeds to end, whether it was through forceful
       means, or intentional ones, all of the initializations done at the top
       are sent to shutdown or destroy, for example freeing up memory allocated
       to loading a model, or properly closing the window & cleaning up some
       things. Normally, it would be okay to simply ignore this and just kill
       the program then and there since the Operating System cleans up after
       ourselves, but it's not a good habit to do so!

       After that's done, Sage returns 0 indicating (hopefully) that everything
       went well.

       For learning more about Sage, there are 2 routes that gives different
       perspectives; a high-level overview of the entire renderer (that is, the
       actual visuals being shown), or the lower-level overview for building up
       to the former.

       For a high-level overview: 'scene.h' provides the introduction to how
       scenes are managed, what is a scene, and what is its components, and how
       are things displayed!

       For a lower-level overview: 'mesh'.h' shows how rendering works using
       OpenGL, what is OpenGL to begin with, and the overall graphics
       pipeline */
    ui_shutdown(&ui);
    scene_destroy(&scene);
    platform_window_shutdown(&platform);

    return 0;
}
