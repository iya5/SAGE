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

#ifndef SAGE_PLATFORM_H
#define SAGE_PLATFORM_H

#include <stdint.h>
#include <stdbool.h>

enum key_code {
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_SPACE,
    KEY_LCTRL,
    KEY_ESC,
    KEY_COUNT
};

enum mouse_button_code {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_COUNT
};

enum polygon_mode {
    POLYGON_LINE,
    POLYGON_FILL
};

struct mouse {
    float x;    /* Current x position */
    float y;    /* Current y position */
    float dx;   /* Delta x since last frame */
    float dy;   /* Delta y since last frame */
    float scroll_x;    /* Horizontal scroll position (possibly unused) */
    float scroll_y;    /* Vertical scroll position */
    bool buttons[MOUSE_COUNT];  /* Button states */
};

struct input_state {
    struct mouse mouse;
    bool keys[KEY_COUNT];
};

struct platform {
    /* Context manages the internal state of the window; this implementation is
       done & managed by GLFW */
    void *context;
    struct input_state input;
    bool running;
    enum polygon_mode draw_mode;
    int32_t viewport_width;
    int32_t viewport_height;

    uint32_t fps;
    float frame_time;
    uint32_t fps_count;
    double fps_timer;
    double dt;
    double current_time;
    double previous_time;
};

/* Initializes platform specific window (GLFW) and stores configurations such as
   viewport sizes, window sizes, and manages input states */
bool platform_window_init(struct platform *platform, 
                          int32_t viewport_width,
                          int32_t viewport_height,
                          int32_t window_width,
                          int32_t window_height);

/* Shutdowns the window and frees related memory */
void platform_window_shutdown(struct platform *platform);

/* Calculates frame timing information like FPS, dt, etc */
void platform_update_frame_timing(struct platform *platform);

/* Polls input; wrapper around glfwPollInput() */
void platform_poll_input(struct platform *platform);

/* Returns the time via GLFW */
double platform_get_time_seconds(void);

/* Swaps the back buffer with the front buffer */
void platform_swap_buffer(struct platform *platform);

/* Wrapper around glfwSetWindowShouldClose() */
bool platform_should_close(struct platform *platform);

/* Wrapper around glPolygonMode() */
void gl_polygon_mode(enum polygon_mode mode);

#endif /* SAGE_PLATFORM_H */
