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


#include "input.h"

void process_input(struct scene *scene, struct platform *platform, double dt)
{
    struct camera *cam = &(scene->cam);
    struct input_state *input = platform->input;
    bool *keys = input->keys;
    bool *mouse_buttons = input->mouse_buttons;

    if (keys[KEY_ESC]) platform->running = false;

    if (keys[KEY_W]) camera_move(cam, MOVE_FORWARD, dt);
    if (keys[KEY_A]) camera_move(cam, STRAFE_LEFT, dt);
    if (keys[KEY_S]) camera_move(cam, MOVE_BACKWARD, dt);
    if (keys[KEY_D]) camera_move(cam, STRAFE_RIGHT, dt);
    if (keys[KEY_SPACE]) camera_move(cam, MOVE_UP, dt);
    if (keys[KEY_LCTRL]) camera_move(cam, MOVE_DOWN, dt);

    if (keys[KEY_1]) gl_polygon_mode(POLYGON_LINE);
    if (keys[KEY_2]) gl_polygon_mode(POLYGON_FILL);

    if (mouse_buttons[MOUSE_RIGHT])
        cam->can_move = true;
    else
        cam->can_move = false;

    camera_mouse(cam, input->mouse_dx, input->mouse_dy);
}
