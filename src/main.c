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
    platform_window_init(&platform, 
                         SAGE_INITIAL_WINDOW_WIDTH,
                         SAGE_INITIAL_WINDOW_HEIGHT,
                         SAGE_INITIAL_VIEWPORT_WIDTH,
                         SAGE_INITIAL_VIEWPORT_HEIGHT);
    ui_init(&ui, platform);
    scene_init(&scene, platform.viewport_width, platform.viewport_height);
    ui_build_scene_graph(&ui.scene_graph, &scene);

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

    ui_shutdown(&ui);
    scene_destroy(&scene);
    platform_window_shutdown(&platform);

    return 0;
}
