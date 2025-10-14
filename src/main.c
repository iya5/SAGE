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

#include "cglm/mat4.h"
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
#include "geometry.h"
#include "texture.h"
#include "math/math.h"

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

struct entity {
    struct vertex_array va;
    struct transform transform;
};

void entity_transform(struct entity entity)
{
    mat4 transform;
    glm_mat4_identity(transform);
    rotate_x(transform, DEG_TO_RAD(entity.transform.rotate[X]), transform);
    rotate_y(transform, DEG_TO_RAD(entity.transform.rotate[Y]), transform);
    rotate_z(transform, DEG_TO_RAD(entity.transform.rotate[Z]), transform);
}

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

struct vertex_array vertex_array_create(const float *vertices, uint32_t vertex_count)
{
    struct vertex_array va = {0};

    uint32_t vao;
    uint32_t vbo;

    glGenVertexArrays(1, &vao);

    // generating buffers for shader
    glGenBuffers(1, &vbo);

    // bind vao
    glBindVertexArray(vao);

    // after binding vao, vbo binds to vao as well
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count, vertices, GL_STATIC_DRAW);

    // bind position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // bind texture uv
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 
        5 * sizeof(float), 
        (void *) (3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // safely unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    va.vao= vao;
    va.vbo = vbo;
    va.vertex_count = vertex_count;

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

    // setting up event callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    struct shader_program program = shader_program_create(
        "res/shaders/default.vert",
        "res/shaders/default.frag"
    );

    struct shader_program light_source_program = shader_program_create(
        "res/shaders/light.vert",
        "res/shaders/light.frag"
    );

    struct shader_program object_program = shader_program_create(
        "res/shaders/object.vert",
        "res/shaders/object.frag"
    );

    struct vertex_array cube = vertex_array_create(
        CUBE_VERTICES,
        sizeof(CUBE_VERTICES)
    );

    shader_program_use(&light_source_program);
    uint32_t light_vao, light_vbo;
    glGenVertexArrays(1, &light_vao);
    glGenBuffers(1, &light_vbo);
    glBindVertexArray(light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LIGHT_CUBE_VERTICES), LIGHT_CUBE_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);

    shader_program_use(&object_program);
    uint32_t object_vao, object_vbo;
    glGenVertexArrays(1, &object_vao);
    glGenBuffers(1, &object_vbo);
    glBindVertexArray(object_vao);
    glBindBuffer(GL_ARRAY_BUFFER, object_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LIGHT_CUBE_N_VERTICES), LIGHT_CUBE_N_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glUseProgram(0);

    struct texture def = texture_create(
        "res/textures/base.png"
    );

    double previous_seconds = glfwGetTime();
    // render loop
    static float rotation_angle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        double current_seconds = glfwGetTime();
        double dt = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        process_input(window, dt);

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shader_program_hot_reload(&program);


        mat4 model;
        glm_mat4_identity(model);
        camera_update(&cam);

        shader_program_use(&program);

        // rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, def.id);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vertex_array_bind(cube);

        for (size_t i = 0; i < 10; i++) {
            for (size_t j = 0; j < 10; j++) {
                mat4 model;
                glm_mat4_identity(model);
                rotate_x(model, DEG_TO_RAD(64), model);
                //glm_rotated_x(model, DEG_TO_RAD(64), model);
                glm_translate(model, (vec3) {(float) i, -0.5, -(float) j});
            
                shader_program_uniform_mat4(program, "model", model);
                shader_program_uniform_mat4(program, "view", cam.view);
                shader_program_uniform_mat4(program, "projection", cam.projection);

                vertex_array_draw(cube);
            }

        }

        // draw the cube being lit on
        shader_program_use(&object_program);
        mat4 light_model;
        glm_mat4_identity(light_model);
        glm_translate(light_model, (vec3) {0.5, 1.5, -1.0});

        rotation_angle += 50.0f * dt; // 50 degrees per second
        if (rotation_angle > 360.0f) rotation_angle -= 360.0f;

        glm_rotate(light_model, glm_rad(rotation_angle), (vec3){1.0f, 0.3f, 0.5f});

        shader_program_uniform_mat4(object_program, "model", light_model);
        shader_program_uniform_mat4(object_program, "view", cam.view);
        shader_program_uniform_mat4(object_program, "projection", cam.projection);

        shader_program_uniform_vec3(object_program, "light_color", (vec3) {1.0, 1.0, 1.0});
        shader_program_uniform_vec3(object_program, "object_color", (vec3) {1.0, 0.5, 0.31});
        shader_program_uniform_vec3(object_program, "light_pos", (vec3) {2.0, 2.2, -3.5});
        shader_program_uniform_vec3(object_program, "view_pos", cam.pos);
        glDrawArrays(GL_TRIANGLES, 0, (sizeof(CUBE_VERTICES) / sizeof(float)) / 3);

        // draw actual light
        shader_program_use(&light_source_program);
        glm_mat4_identity(light_model);
        glm_translate(light_model, (vec3) {2.0, 2.2, -3.5});
        shader_program_uniform_mat4(light_source_program, "model", light_model);
        shader_program_uniform_mat4(light_source_program, "view", cam.view);
        shader_program_uniform_mat4(light_source_program, "projection", cam.projection);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(CUBE_VERTICES) / sizeof(float));
        // swap buffers and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vertex_array_free(&cube);
    shader_program_destroy(&program);
    glfwTerminate();
    window = NULL;

    return 0;
}
