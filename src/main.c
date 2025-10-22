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

#include "mnf/mnf.h"
#include "mnf/mnf_matrix.h"
#include "mnf/mnf_transform.h"
#define NK_IMPLEMENTATION
#include <nuklear.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <slog/slog.h>
//#include <cglm/cglm.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "camera.h"
#include "shader/shader.h"
#include "geometry.h"
#include "texture.h"

struct vertex_array {
    uint32_t vao;
    uint32_t vbo;
    uint32_t vertex_count;
};

struct transform {
    vec3 rotate;
    vec3 scale;
    vec3 position;
};

struct camera cam = {0};


void error_callback(int32_t error, const char *description)
{
    LOG_FATAL("Error (%d): %s", error, description);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window, double dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        cam.can_move = true;
    } else {
        cam.can_move = false;
    }
}

struct vertex_array vertex_array_create(const float *vertices,
                                        size_t n_bytes)
{
    struct vertex_array va = {0};

    uint32_t vao;
    uint32_t vbo;

    // generating buffers for vertex
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind vao
    glBindVertexArray(vao);

    // bind and copy data over to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, n_bytes, vertices, GL_STATIC_DRAW);

    // configure the vbo to interpret the data
    // stride is the offset between consecutive generic vertex attributes
    size_t stride = 5 * sizeof(float);
    GLboolean normalized = GL_FALSE;

    // bind position attributes
    uint32_t pos_index = 0;
    int32_t pos_size = 3;
    void *pos_offset = (void *) 0;
    glVertexAttribPointer(pos_index,
                          pos_size,
                          GL_FLOAT,
                          normalized,
                          stride,
                          pos_offset);
    glEnableVertexAttribArray(pos_index);

    // bind texture uv attributes
    uint32_t uv_index = 1;
    int32_t uv_size = 2;
    void *uv_offset = (void *) (pos_size * sizeof(float));
    glVertexAttribPointer(uv_index,
                          uv_size,
                          GL_FLOAT,
                          normalized, 
                          stride,
                          uv_offset);
    glEnableVertexAttribArray(uv_index);

    // safely unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    va.vao= vao;
    va.vbo = vbo;
    va.vertex_count = (uint32_t) ((n_bytes) / sizeof(vertices[0]));
    LOG_DEBUG("VA created, number of vertices: %d", va.vertex_count);

    return va;
}

void vertex_array_free(struct vertex_array *va)
{
    LOG_INFO("Destroying vertex array buffers");
    glDeleteVertexArrays(1, &(va->vao));
    glDeleteBuffers(1, &(va->vbo));
    va->vao = 0;
    va->vbo = 0;
    va->vertex_count = 0;
}

void vertex_array_bind(struct vertex_array va)
{
    glBindVertexArray(va.vao);
}

void vertex_array_draw(struct vertex_array va)
{
    glDrawArrays(GL_TRIANGLES, 0, va.vertex_count);
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

void world_grid_draw(struct vertex_array va, 
                     struct shader shader, 
                     struct texture texture,
                     struct camera cam)
{
    // prolly a naive approach at drawing a world grid, could do this instead
    // in the shaders. currently implemented as drawing 3 seperate strecthed out
    // cubes representing the cardinal axis
    shader_use(&shader);
    texture_bind(texture);
    vertex_array_bind(va);

    mat4 model;

    // x axis (red)
    mnf_mat4_identity(model);
    mnf_mat4_scale(model, (vec3){200.0, 0.01, 0.01}, model);
    shader_uniform_mat4(shader, "model", model);
    shader_uniform_mat4(shader, "view", cam.view);
    shader_uniform_mat4(shader, "projection", cam.projection);
    shader_uniform_vec4(shader, "color", (vec4){1.0, 0.0, 0.0, 1.0});
    vertex_array_draw(va);

    // y axis (green)
    mnf_mat4_identity(model);
    mnf_mat4_scale(model, (vec3){200.0, 0.01, 0.01}, model);
    mnf_euler_rotate_y(model, MNF_RAD(90), model);
    shader_uniform_mat4(shader, "model", model);
    shader_uniform_mat4(shader, "view", cam.view);
    shader_uniform_mat4(shader, "projection", cam.projection);
    shader_uniform_vec4(shader, "color", (vec4){0.0, 1.0, 0.0, 1.0});
    vertex_array_draw(va);

    // z axis (blue)
    mnf_mat4_identity(model);
    mnf_mat4_scale(model, (vec3){200.0, 0.01, 0.01}, model);
    mnf_euler_rotate_z(model, MNF_RAD(90), model);
    mnf_mat4_translate(model, (vec3){0.0, 100.0, 0.0}, model);
    shader_uniform_mat4(shader, "model", model);
    shader_uniform_mat4(shader, "view", cam.view);
    shader_uniform_mat4(shader, "projection", cam.projection);
    shader_uniform_vec4(shader, "color", (vec4){0.0, 0.0, 1.0, 1.0});
    vertex_array_draw(va);
}

int main(int argc, [[maybe_unused]] char **argv)
{
    if (argc > 1) {
        LOG_FATAL("usage: bin/sage");
        exit(1);
    }


    GLFWwindow *window = NULL;

    LOG_INFO("Starting GLFW: %s", glfwGetVersionString());
    glfwSetErrorCallback(error_callback);

    // GLFW Library initialization
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SAGE_OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SAGE_OPENGL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, SAGE_WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        LOG_FATAL("GLFW failed to create a window");
        glfwTerminate();
        window = NULL;
        return -1;
    }

    glfwMakeContextCurrent(window);

    // setting up event callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        LOG_FATAL("Failed to initialize OpenGL context");
        return -1;
    }
    LOG_INFO("Loaded OpenGL %d.%d\n", 
             GLAD_VERSION_MAJOR(version),
             GLAD_VERSION_MINOR(version));

    // setup MSAA
    glfwWindowHint(GLFW_SAMPLES, SAGE_MULTISAMPLE_ANTIALIASING);
    glfwSwapInterval(SAGE_VSYNC_SETTING);

    glViewport(0, 0, 640, 480);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int n_vertex_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n_vertex_attributes);
    LOG_INFO("Maximum vertex attributes supported: %d", n_vertex_attributes);

    float aspect = 640.0 / 480.0;

    camera_init(&cam, CAM_DEFAULT_POS, CAM_DEFAULT_FORWARD, CAM_DEFAULT_UP);
    camera_perspective(&cam, 
        FOV_DEFAULT, 
        aspect, 
        PERSPECTIVE_DEFAULT_NEAR, 
        PERSPECTIVE_DEFAULT_FAR
    );

    struct shader basic_shader = shader_create("shaders/basic.glsl");
    struct shader light_shader = shader_create("shaders/light.glsl");
    struct shader lit_shader = shader_create("shaders/lit.glsl");

    // it's necessary to pass cube vertices size because arrays decay into 
    // pointers
    struct vertex_array cube = vertex_array_create(
        CUBE_VERTICES,
        sizeof(CUBE_VERTICES)
    );

    struct vertex_array light_source = vertex_array_create(
        CUBE_VERTICES,
        sizeof(CUBE_VERTICES)
    );

    struct texture base_texture = texture_create("res/textures/base.png");
    struct texture default_texture = texture_create_default();

    double previous_seconds = glfwGetTime();

    // render loop
    while (!glfwWindowShouldClose(window)) {
        double current_seconds = glfwGetTime();
        double dt = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        process_input(window, dt);

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shader_hot_reload(&basic_shader);


        camera_update(&cam);

        // rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world_grid_draw(cube, basic_shader, default_texture, cam);

        /* drawing light source */
        shader_use(&light_shader);
        texture_bind(default_texture);
        vec3 light_pos = {2.0, 3.0, -1.0};
        mat4 model;
        mnf_mat4_identity(model);
        mnf_mat4_translate(model, light_pos, model);
        shader_uniform_mat4(light_shader, "model", model);
        shader_uniform_mat4(light_shader, "view", cam.view);
        shader_uniform_mat4(light_shader, "projection", cam.projection);
        vertex_array_draw(light_source);

        /* drawing lit object */
        shader_use(&lit_shader);
        texture_bind(base_texture);
        mnf_mat4_identity(model);
        shader_uniform_vec3(lit_shader, "object_color", (vec3){1.0, 0.5, 0.31});
        shader_uniform_vec3(lit_shader, "light_color", (vec3){1.0, 1.0, 1.0});
        shader_uniform_mat4(lit_shader, "model", model);
        shader_uniform_mat4(lit_shader, "view", cam.view);
        shader_uniform_mat4(lit_shader, "projection", cam.projection);
        vertex_array_draw(cube);

        /*
        for (size_t i = 0; i < 10; i++) {
            for (size_t j = 0; j < 10; j++) {
                texture_bind(base_texture);
                shader_use(&basic_shader);
                vertex_array_bind(cube);

                mat4 model;

                mnf_mat4_identity(model);
                mnf_mat4_scale(model, (vec3){1.0, 1.0, 1.0}, model);

                mnf_mat4_translate(model, (vec3) {(float) i, 0, -(float) j}, model);
            
                shader_uniform_mat4(basic_shader, "model", model);
                shader_uniform_vec4(basic_shader, "color", (vec4){1.0, 1.0, 1.0, 1.0});
                shader_uniform_mat4(basic_shader, "view", cam.view);
                shader_uniform_mat4(basic_shader, "projection", cam.projection);

                vertex_array_draw(cube);
            }

        }
        */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vertex_array_free(&cube);

    shader_destroy(&basic_shader);

    glfwTerminate();
    window = NULL;

    return 0;
}
