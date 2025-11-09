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

#include <stdlib.h>
#include <string.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "platform.h"
#include "config.h"
#include "logger.h"

#define GET_KPD(action) (action) == GLFW_PRESS || (action) == GLFW_REPEAT

void error_callback(int32_t error, const char *description)
{
    SERROR("Error (%d): %s", error, description);
}

void framebuffer_size_callback(GLFWwindow *window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
    struct platform *platform = glfwGetWindowUserPointer(window);
    if (platform == NULL) return;
    platform->viewport_width = width;
    platform->viewport_height = height;
}

void scroll_callback(GLFWwindow *window, double dx, double dy)
{
    /* camera_scroll(&cam, dy); */
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    struct platform *platform = glfwGetWindowUserPointer(window);
    bool *mouse_buttons = platform->input->mouse_buttons;

    if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
        if (button == GLFW_MOUSE_BUTTON_1) mouse_buttons[MOUSE_LEFT] = GET_KPD(action);
        if (button == GLFW_MOUSE_BUTTON_2) mouse_buttons[MOUSE_RIGHT] = GET_KPD(action);
    }
}

void key_callback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    struct platform *platform = glfwGetWindowUserPointer(window);
    bool *keys = platform->input->keys;

    if (key >= 0 && key <= GLFW_KEY_LAST) {
        if (key == GLFW_KEY_A) keys[KEY_A] = GET_KPD(action);
        if (key == GLFW_KEY_W) keys[KEY_W] = GET_KPD(action);
        if (key == GLFW_KEY_S) keys[KEY_S] = GET_KPD(action);
        if (key == GLFW_KEY_D) keys[KEY_D] = GET_KPD(action);
        if (key == GLFW_KEY_SPACE) keys[KEY_SPACE] = GET_KPD(action);
        if (key == GLFW_KEY_SPACE) keys[KEY_SPACE] = GET_KPD(action);
        if (key == GLFW_KEY_LEFT_CONTROL) keys[KEY_LCTRL] = GET_KPD(action);
        if (key == GLFW_KEY_ESCAPE) keys[KEY_ESC] = GET_KPD(action);
        if (key == GLFW_KEY_0) keys[KEY_0] = GET_KPD(action);
        if (key == GLFW_KEY_1) keys[KEY_1] = GET_KPD(action);
        if (key == GLFW_KEY_2) keys[KEY_2] = GET_KPD(action);
        if (key == GLFW_KEY_3) keys[KEY_3] = GET_KPD(action);
        if (key == GLFW_KEY_4) keys[KEY_4] = GET_KPD(action);
    }
}

void platform_poll_input(struct platform *platform)
{
    glfwPollEvents();

    /* Idk if it's good to poll for mouse in the software or just react to
     * events, but i'll fill input queue for key presses using events instead
     * because they're rarely pressed compared to mouse movement
     */
    struct input_state *input = platform->input;

    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(platform->context, &mouse_x, &mouse_y);
    input->mouse_dx = (float) mouse_x - input->mouse_x;
    input->mouse_dy = input->mouse_y - (float) mouse_y;
    input->mouse_x = (float) mouse_x;
    input->mouse_y = (float) mouse_y;
}

bool platform_window_init(struct platform *platform, 
                          int32_t viewport_width,
                          int32_t viewport_height,
                          int32_t window_width,
                          int32_t window_height)
{
    struct input_state *input = NULL;

    SINFO("Starting GLFW: %s", glfwGetVersionString());

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        SFATAL("GLFW failed to initialize");
        exit(1);
    }


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *context = glfwCreateWindow(window_width,
                                           window_height,
                                           SAGE_WINDOW_TITLE,
                                           NULL,
                                           NULL);
    if (context == NULL) {
        SFATAL("GLFW failed to create a window");
        goto err_platform;
    }

    glfwMakeContextCurrent(context);

    /* setup hints */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SAGE_OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SAGE_OPENGL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, SAGE_MULTISAMPLE_ANTIALIASING);
    glfwSwapInterval(SAGE_VSYNC_SETTING);

    /* setting up event callbacks */
    glfwSetFramebufferSizeCallback(context, framebuffer_size_callback);
    glfwSetScrollCallback(context, scroll_callback);
    glfwSetKeyCallback(context, key_callback);
    glfwSetMouseButtonCallback(context, mouse_button_callback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        SFATAL("Failed to load OpenGL functions. Check your graphics drivers");
        goto err_platform;
    }

    SINFO("Loaded OpenGL %d.%d", 
          GLAD_VERSION_MAJOR(version),
          GLAD_VERSION_MINOR(version));


    input = (struct input_state *) malloc(sizeof(struct input_state));
    if (input == NULL) {
        SFATAL("Could not alloc memory for platform state");
        goto err_platform;
    }
    memset(input, 0, sizeof(struct input_state));

    /* fetching GPU specs */
    int n_vertex_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_vertex_attributes);

    int32_t max_vertex_texture_units;
    int32_t max_fragment_texture_units;
    int32_t max_geometry_texture_units;
    int32_t max_combined_texture_units;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_vertex_texture_units);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_fragment_texture_units);
    glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &max_geometry_texture_units);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_units);

    SINFO("GPU specifications provided by OpenGL:");
    SINFO("\tMax vertex shader texture units: %d", max_vertex_texture_units);
    SINFO("\tMax fragment shader texture units: %d", max_fragment_texture_units);
    SINFO("\tMax geometry shader texture units: %d", max_geometry_texture_units);
    SINFO("\tMax combined shader texture units: %d", max_combined_texture_units);
    SINFO("\tMax vertex shader attributes: %d", n_vertex_attributes);

    platform->input = input;
    platform->context = context;
    platform->running = true;
    platform->viewport_width = viewport_width;
    platform->viewport_height = viewport_height;

    /* set user pointer so platform is accessible in callbacks */
    glfwSetWindowUserPointer(context, platform);

    return true;

err_platform:
    if (input) free(input);
    glfwTerminate();
    context = NULL;
    return false;
}

double platform_get_time(void)
{
    return glfwGetTime();
}

bool platform_should_close(struct platform *platform)
{
    return (glfwWindowShouldClose(platform->context) || !platform->running);
}

void platform_swap_buffer(struct platform *platform)
{
    glfwSwapBuffers(platform->context);
}

void platform_window_shutdown(struct platform *platform)
{
    SINFO("Closing window and cleaning up");
    glfwTerminate();
    free(platform->input);
    platform->context = NULL;
    platform->running = false;
    platform->viewport_width = 0;
    platform->viewport_height = 0;
}
