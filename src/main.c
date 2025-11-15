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
#include <stdlib.h>

#include "camera.h"
#include "config.h"
#include "scene.h"
#include "logger.h"
#include "platform.h"

struct scene scene;
struct platform platform;

bool SAGE_DRAW_SKYBOX = false;

void process_input(double dt)
{
    struct camera *cam = &(scene.cam);
    struct input_state *input = platform.input;
    bool *keys = input->keys;
    bool *mouse_buttons = input->mouse_buttons;

    if (keys[KEY_ESC])
        platform.running = false;

    if (keys[KEY_W])
        camera_move(cam, MOVE_FORWARD, dt);

    if (keys[KEY_A])
        camera_move(cam, STRAFE_LEFT, dt);

    if (keys[KEY_S])
        camera_move(cam, MOVE_BACKWARD, dt);

    if (keys[KEY_D])
        camera_move(cam, STRAFE_RIGHT, dt);

    if (keys[KEY_SPACE])
        camera_move(cam, MOVE_UP, dt);

    if (keys[KEY_LCTRL])
        camera_move(cam, MOVE_DOWN, dt);

    if (keys[KEY_1])
        SAGE_DRAW_SKYBOX = true;

    if (keys[KEY_2])
        SAGE_DRAW_SKYBOX = false;

    if (keys[KEY_3])
        gl_polygon_mode(POLYGON_LINE);

    if (keys[KEY_4])
        gl_polygon_mode(POLYGON_FILL);

    if (mouse_buttons[MOUSE_RIGHT])
        cam->can_move = true;
    else
        cam->can_move = false;

    camera_mouse(cam, input->mouse_dx, input->mouse_dy);
}

int main(int argc, __attribute__((unused)) char **argv)
{
    if (argc > 1) {
        SFATAL("Sage has no flags, run the binary by itself.");
        exit(1);
    }

    platform_window_init(&platform, 
                         SAGE_INITIAL_WINDOW_WIDTH,
                         SAGE_INITIAL_WINDOW_HEIGHT,
                         SAGE_INITIAL_VIEWPORT_WIDTH,
                         SAGE_INITIAL_VIEWPORT_HEIGHT);
    
    scene_init(&scene, platform.viewport_width, platform.viewport_height);

    double previous_time = platform_get_time_seconds();
    while (!platform_should_close(&platform)) {
        double current_time = platform_get_time_seconds();
        double delta_time = current_time - previous_time;
        previous_time = current_time;

        platform_poll_input(&platform);
        process_input(delta_time);

        scene_render(&scene);

        platform_swap_buffer(&platform);
    }

    scene_destroy(&scene);
    platform_window_shutdown(&platform);

    return 0;
}
