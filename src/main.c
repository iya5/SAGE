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

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <cimgui.h>
#include <cimgui_impl.h>

#include "math/math.h"

#include "config.h"
#include "shader/shader.h"


struct mesh {
    uint32_t handle;
    uint32_t vbo;
    uint32_t ebo;
};

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

void perspective_projection(mat4 projection,
                            float near, float far,
                            float range,
                            float aspect_ratio,
                            float fov)
{
    /*
     * Projection matrix looks like this (in column major)
     *
     * [Sx,  0,  0,  0]
     * [ 0, Sy,  0,  0]
     * [ 0,  0, Sz, Pz]
     * [ 0,  0, -1,  0]
     *
     * S is just a vector that scales its respective components x, y, z
     * Pz adjusts points' range between clipping planes (the near and far plane
     * of the view frustrum
     * the -1 swaps the vector's fourth component 'w' with its negated
     * third 'z' component used for perspective division
     */

    vec3 scale = {
        (2 * near) / (range * aspect_ratio + range * aspect_ratio),
        near / range,
        -(far + near) / (far - near)
    };

    // what exactly is that name Pz?
    float pz = -(2 * far * near) / (far - near);
    range = tan(fov * 0.5) * near;

    mat4 temp = {
        {scale[X], 0, 0, 0},
        {0, scale[Y], 0, 0},
        {0, 0, scale[Z], -1},
        {0, 0, pz, 0}
    };
    glm_mat4_copy(temp, projection);
}

void camera_lookat(mat4 view, 
                   vec3 camera_pos, 
                   vec3 target_pos, 
                   vec3 up_direction)
{
    vec3 distance_vector;
    glm_vec3_copy(
        (vec3) {
            target_pos[X] - camera_pos[X],
            target_pos[Y] - camera_pos[Y],
            target_pos[Z] - camera_pos[Z]
        },
        distance_vector
    );

    /*
     * A view matrix typically looks like this (in column major)
     *
     * [ Rx,  Ry,  Rz, -Px]
     * [ Ux,  Uy,  Uv, -Py]
     * [-Fx, -Fy, -Fz, -Pz]
     * [  0,   0,   0,   1]
     *
     * Where U is a unit vector pointing upwards
     * Where F is forwards
     * Where R is to the right
     * and P is the position of the camera relative to the world space
     *
     * Since U handles pointing upwards, it is the camera pitch and yaw
     */

    // normalize the distance vector by calculating the magnitude and
    // multiplying the vector by it
    // this is done so that our forward direction vector has a length of
    // 1 unit
    //
    // Why is P the negated world position of the camera?
    //
    // Why is Forward negated as well?
    float magnitude = sqrtf(
        distance_vector[X] * distance_vector[X] + \
        distance_vector[Y] * distance_vector[Y] + \
        distance_vector[Z] * distance_vector[Z]
    );

    // normalizing the distance vector gives the forward unit vector
    vec3 forward_vector;
    glm_vec3_copy(
        (vec3) {
            distance_vector[X] / magnitude,
            distance_vector[Y] / magnitude,
            distance_vector[Z] / magnitude
        },
        forward_vector
    );

    /* finding the Right vector means to find the vector perpendicular
     * to both forward and upward, which is to the right(or left but right
     * because left hand coordinate!)
     * this is done by caculating the cross product of r^ = F x U^
     *
     * what order should it be done?, 
     */
    
    // R^ = F^ x U^
    vec3 right_vector;

    glm_vec3_copy(
        (vec3) {
            forward_vector[Y] * up_direction[Z] - forward_vector[Z] * up_direction[Y],
            forward_vector[Z] * up_direction[X] - forward_vector[X] * up_direction[Z],
            forward_vector[X] * up_direction[Y] - forward_vector[Y] * up_direction[X]
        },
        right_vector
    );

    // recaculating the up vector for consistency
    // U^ = r^ x F^
    up_direction[X] = right_vector[Y] * forward_vector[Z] - right_vector[Z] * forward_vector[Y];
    up_direction[Y] = right_vector[Z] * forward_vector[X] - right_vector[X] * forward_vector[Z];
    up_direction[Z] = right_vector[X] * forward_vector[Y] - right_vector[Y] * forward_vector[X];

    glm_mat4_identity(view);
    mat4 temp = {
        {right_vector[X], up_direction[X], -forward_vector[X], 0},
        {right_vector[Y], up_direction[Y], -forward_vector[Y], 0},
        {right_vector[Z], up_direction[Z], -forward_vector[Z], 0},
        {-camera_pos[X], -camera_pos[Y], -camera_pos[Z], 1}
    };
    glm_mat4_copy(temp, view);
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

    // setup MSAA
    glfwWindowHint(GLFW_SAMPLES, SAGE_MULTISAMPLE_ANTIALIASING);
    glfwSwapInterval(SAGE_VSYNC_SETTING);

    window = glfwCreateWindow(640, 480, SAGE_WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        LOG_FATAL("GLFW failed to create a window");
        glfwTerminate();
        window = NULL;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGL()) {
        LOG_FATAL("Failed to load and initialize GLAD");
        return -1;
    }

    glViewport(0, 0, 640, 480);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // disappears when these are turned on
    // glDepthFunc(GL_LESS);
    // glEnable(GL_DEPTH_TEST);

    struct mesh quad = mesh_create_quad();
    // struct mesh triangle = mesh_create_triangle();

    struct shader_program program = shader_program_create(
        "res/shader/shader.vert",
        "res/shader/shader.frag"
    );

    float cam_speed = 1.0;
    float cam_yaw_speed = 10.0; // 10 degrees per second
    
    // OpenGL has -z as towards the screen and +z as towards the user
    vec3 cam_pos = {0.0, 0.0, 2.0};
    float cam_yaw = 0.0;

    double previous_seconds = glfwGetTime();
    // render loop
    while (!glfwWindowShouldClose(window)) {
        double current_seconds = glfwGetTime();
        double delta = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shader_program_hot_reload(&program);

        // camera
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam_pos[X] -= cam_speed * delta;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam_pos[X] += cam_speed * delta;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam_pos[Z] -= cam_speed * delta;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam_pos[Z] += cam_speed * delta; 
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cam_pos[Y] += cam_speed * delta; 
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cam_pos[Y] -= cam_speed * delta; 

        shader_program_use(&program);

        shader_program_uniform_vec4(
            program, "color", 
            (vec4) {0.2, 0.1, 0.2, 1.0}
        );

        mat4 model, view, projection;
        glm_mat4_identity(model);
        shader_program_uniform_mat4(program, "model", model);

        camera_lookat(view, cam_pos, (vec3) {0, 0, 0}, (vec3) {0, 1, 0});
        shader_program_uniform_mat4(program, "view", view);
        
        float near = 0.1;
        float far = 100.0;
        float fov = 67.0 * (2.0 * 3.14278) / 360.0;
        float aspect = 640.0 / 480.0;

        float range = tan(fov * 0.5) * near;
        perspective_projection(
            projection, 
            near, 
            far, 
            range, 
            aspect,
            fov
        );
        shader_program_uniform_mat4(program, "projection", projection);

        // renderingArray
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(quad.handle);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // swap buffers and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }

    mesh_destroy(quad);
    shader_program_destroy(&program);
    glfwTerminate();
    window = NULL;

    return 0;
}
