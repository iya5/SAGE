/* Entry point for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

//#include <cimgui.h>
//#include <cimgui_impl.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <slog/slog.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <cimgui.h>
#include <cimgui_impl.h>

#include "config.h"
#include "camera.h"
#include "shader/shader.h"
struct mesh {
    uint32_t handle;
    uint32_t vbo;
    uint32_t ebo;
};

struct camera cam = {0};

void error_callback(int32_t error, const char *description)
{
    fprintf(stderr, "Error (%d): %s\n", error, description);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

struct mesh mesh_create_triangle(void)
{
    struct mesh triangle = {0};
    // now onto the vertices
    // rendering a triangle
    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.0, 0.5, 0.0
    };

    // generating a vao first
    uint32_t vao;
    uint32_t vertex_vbo;

    glGenVertexArrays(1, &vao);
    // generating buffers for shader
    glGenBuffers(1, &vertex_vbo);

    // bind vao
    glBindVertexArray(vao);

    // after binding vao, vbo binds to vao as well
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(0);

    // safely unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    triangle.handle = vao;

    return triangle;
}

struct mesh mesh_create_cube(void)
{
    struct mesh cube = {0};
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,//  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,//  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,//  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,//  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,//  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,//  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,//  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,//  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,//  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,//  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,//  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,//  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,//  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,//  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,//  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,//  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,//  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,//  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f//  0.0f, 1.0f
    };

    uint32_t vao;
    uint32_t point_vbo;

    glGenVertexArrays(1, &vao);

    // generating buffers for shader
    glGenBuffers(1, &point_vbo);

    // bind vao
    glBindVertexArray(vao);

    // after binding vao, vbo binds to vao as well
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(0);

    // bind color buffer

    // safely unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    cube.handle = vao;
    cube.vbo = point_vbo;

    return cube;
}

struct mesh mesh_create_quad(void)
{
    struct mesh quad = {0};
    float vertices[] = {
        // first triangle
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0,

        // second triangle
        0.5, -0.5, 0.0,
        0.5, 0.5, 0.0,
        -0.5, 0.5, 0.0
    };

    uint32_t vao;
    uint32_t point_vbo;

    glGenVertexArrays(1, &vao);

    // generating buffers for shader
    glGenBuffers(1, &point_vbo);

    // bind vao
    glBindVertexArray(vao);

    // after binding vao, vbo binds to vao as well
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    glEnableVertexAttribArray(0);

    // bind color buffer

    // safely unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    quad.handle = vao;
    quad.vbo = point_vbo;

    return quad;
}

void mesh_destroy(struct mesh m)
{
    LOG_INFO("Destroying mesh vertex buffers");
    glDeleteVertexArrays(1, &m.handle);
    glDeleteBuffers(1, &m.vbo);
}

void mouse_callback([[maybe_unused]] GLFWwindow *window, double x_pos, double y_pos)
{
    static bool first_mouse = true;
    static float last_x = 640.0 / 2.0;
    static float last_y = 480.0 / 2.0;

    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float dx = x_pos - last_x;
    float dy = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;
    camera_mouse(&cam, dx, dy);
}

void scroll_callback([[maybe_unused]] GLFWwindow *window, 
                     [[maybe_unused]] double dx, 
                     double dy)
{
    camera_scroll(&cam, dy);
}

int main(int argc, [[maybe_unused]] char **argv)
{
    if (argc > 1) {
        LOG_FATAL("usage: bin/sage");
        exit(1);
    }

    LOG_INFO("Starting GLFW: %s", glfwGetVersionString());

    GLFWwindow *window = NULL;


    glfwSetErrorCallback(error_callback);

    // GLFW Library initialization
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(640, 480, SAGE_WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        LOG_FATAL("GLFW failed to create a window");
        glfwTerminate();
        window = NULL;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL()) {
        LOG_FATAL("Failed to load and initialize GLAD");
        return -1;
    }
    // setup MSAA
    glfwWindowHint(GLFW_SAMPLES, SAGE_MULTISAMPLE_ANTIALIASING);
    glfwSwapInterval(SAGE_VSYNC_SETTING);

    glViewport(0, 0, 640, 480);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    struct mesh cube= mesh_create_cube();
    // struct mesh triangle = mesh_create_triangle();

    float aspect = 640.0 / 480.0;

    camera_init(&cam, CAM_DEFAULT_POS, CAM_DEFAULT_FORWARD, CAM_DEFAULT_UP);
    camera_perspective(
        &cam, 
        FOV_DEFAULT, 
        aspect, 
        PERSPECTIVE_DEFAULT_NEAR, 
        PERSPECTIVE_DEFAULT_FAR
    );

    struct shader_program program = shader_program_create(
        "res/shader/shader.vert",
        "res/shader/shader.frag"
    );


    double previous_seconds = glfwGetTime();
    // render loop
    while (!glfwWindowShouldClose(window)) {
        double current_seconds = glfwGetTime();
        double dt = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        // input_process(int KEY);

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shader_program_hot_reload(&program);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera_move(&cam, MOVE_FORWARD, dt);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera_move(&cam, MOVE_BACKWARD, dt);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera_move(&cam, STRAFE_LEFT, dt);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_move(&cam, STRAFE_RIGHT, dt);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera_move(&cam, MOVE_UP, dt);

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            camera_move(&cam, MOVE_DOWN, dt);


        shader_program_use(&program);

        shader_program_uniform_vec4(
            program, "color", 
            (vec4) {0.2, 0.1, 0.2, 1.0}
        );

        mat4 model;
        glm_mat4_identity(model);

        camera_update(&cam);
        
        shader_program_uniform_mat4(program, "model", model);
        shader_program_uniform_mat4(program, "view", cam.view);
        shader_program_uniform_mat4(program, "projection", cam.projection);


        // renderingArray
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cube.handle);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // swap buffers and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }

    mesh_destroy(cube);
    shader_program_destroy(&program);
    glfwTerminate();
    window = NULL;

    return 0;
}
