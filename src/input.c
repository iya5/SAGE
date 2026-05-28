#include "input.h"
#include "platform.h"

void process_input(struct scene *scene, struct platform *platform)
{
    double dt = platform->dt;
    struct camera *cam = &(scene->cam);
    struct mouse mouse = platform->input.mouse;
    bool *keys = platform->input.keys;

    if (keys[KEY_ESC]) platform->running = false;

    if (keys[KEY_W]) camera_move(cam, MOVE_FORWARD, dt);
    if (keys[KEY_A]) camera_move(cam, STRAFE_LEFT, dt);
    if (keys[KEY_S]) camera_move(cam, MOVE_BACKWARD, dt);
    if (keys[KEY_D]) camera_move(cam, STRAFE_RIGHT, dt);
    if (keys[KEY_SPACE]) camera_move(cam, MOVE_UP, dt);
    if (keys[KEY_LCTRL]) camera_move(cam, MOVE_DOWN, dt);

    if (keys[KEY_1]) platform->draw_mode = POLYGON_FILL;
    if (keys[KEY_2]) platform->draw_mode = POLYGON_LINE;

    if (mouse.buttons[MOUSE_RIGHT])
        cam->can_move = true;
    else
        cam->can_move = false;

    camera_mouse(cam, mouse.dx, mouse.dy);
}
