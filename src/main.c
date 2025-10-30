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

#include "mnf/mnf_matrix.h"
#ifndef VERSION
#define VERSION "Something went wrong with VERSION"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

/*
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#include <nuklear.h>
#include <demo/glfw_opengl4/nuklear_glfw_gl4.h>
*/

#include "mnf/mnf.h" // IWYU pragma: keep
#include "slog/slog.h"
#include "config.h"
#include "camera.h"
#include "shader.h"
#include "geometry.h"
#include "texture.h"
#include "mesh.h"

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

void world_grid_draw(struct mesh mesh,
                     struct shader shader, 
                     struct texture texture,
                     struct camera cam)
{
    // prolly a naive approach at drawing a world grid, could do this instead
    // in the shaders. currently implemented as drawing 3 seperate strecthed out
    // cubes representing the cardinal axis
    shader_use(&shader);
    texture_bind(texture);
    mesh_bind(mesh);

    // x axis (red)
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){200.0, 0.01, 0.01});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){1.0, 0.0, 0.0, 1.0});
    mesh_draw(mesh);

    // y axis (green)
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){0.01, 200.0, 0.01});
    mesh_set_rotation(&mesh, (vec3){0, MNF_RAD(90), 0});
    mesh_set_position(&mesh, (vec3){0.0, 100.0, 0.0});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){0.0, 1.0, 0.0, 1.0});
    mesh_draw(mesh);

    // z axis (blue)
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){0.01, 0.01, 200.0});
    mesh_set_rotation(&mesh, (vec3){0, 0, MNF_RAD(90)});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){0.0, 0.0, 1.0, 1.0});
    mesh_draw(mesh);
}

int main(int argc, [[maybe_unused]] char **argv)
{
    if (argc > 1) {
        LOG_FATAL("usage: bin/sage");
        exit(1);
    }

    LOG_INFO("Hello world from Sage '%s'!", VERSION);

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

    /* setup gl parameters */
    glViewport(0, 0, 640, 480);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
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
    struct shader color_shader = shader_create("shaders/color.glsl");
    struct shader skybox_shader = shader_create("shaders/skybox.glsl");
    struct shader light_shader = shader_create("shaders/light.glsl");
    struct shader lit_shader = shader_create("shaders/phong.glsl");

    // it's necessary to pass cube vertices size because arrays decay into 
    // pointers
    struct mesh cube = mesh_create(CUBE_VERTEX_ARRAY,
                                   sizeof(CUBE_VERTEX_ARRAY));
    struct mesh light_source = mesh_create(CUBE_VERTEX_ARRAY,
                                   sizeof(CUBE_VERTEX_ARRAY));
    struct mesh skybox = mesh_create(CUBE_VERTEX_ARRAY,
                                     sizeof(CUBE_VERTEX_ARRAY));

    struct texture base_texture = texture_create("res/textures/base.png");
    //struct texture uv_grid_texture = texture_create("res/textures/uv-grid.jpg");
    struct texture default_texture = texture_create_default();

    /*
    char *cubemap_faces[6] = {
        "res/textures/daylight-cubemap/right.bmp",
        "res/textures/daylight-cubemap/left.bmp",
        "res/textures/daylight-cubemap/top.bmp",
        "res/textures/daylight-cubemap/bottom.bmp",
        "res/textures/daylight-cubemap/front.bmp",
        "res/textures/daylight-cubemap/back.bmp",
    };
    */
    char *cubemap_faces[6] = {
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg",
    };

    struct texture cubemap = cubemap_texture_create(cubemap_faces);

    double previous_seconds = glfwGetTime();

    /*
    struct nk_context *context = nk_glfw3_init(
        window, 
        NK_GLFW3_INSTALL_CALLBACKS,
        MAX_VERTEX_BUFFER,
        MAX_ELEMENT_BUFFER
    );

    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    nk_glfw3_font_stash_end();
    */

    // render loop
    //mesh_set_scale(&skybox, (vec3){100.0, 100.0, 100.0,});
    //mesh_update_transform(&skybox);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double current_seconds = glfwGetTime();
        double dt = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        process_input(window, dt);

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shader_hot_reload(&basic_shader);

        /*
        nk_glfw3_new_frame();

        if (nk_begin(context, "Nuklear window", nk_rect(0, 0, 500, 500),
                     NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)) {

            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(context, 30, 80, 1);
            if (nk_button_label(context, "button"))
                LOG_DEBUG("button pressed");

            nk_layout_row_dynamic(context, 30, 2);
            if (nk_option_label(context, "easy", op == EASY)) op = EASY;
            if (nk_option_label(context, "hard", op == HARD)) op = HARD;

            nk_layout_row_dynamic(context, 25, 1);
            nk_property_int(context, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(context, 20, 1);
            nk_label(context, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(context, 25, 1);
            if (nk_combo_begin_color(context, nk_rgb_cf(bg), nk_vec2(nk_widget_width(context),400))) {
                nk_layout_row_dynamic(context, 120, 1);
                bg = nk_color_picker(context, bg, NK_RGBA);
                nk_layout_row_dynamic(context, 25, 1);
                bg.r = nk_propertyf(context, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                bg.g = nk_propertyf(context, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                bg.b = nk_propertyf(context, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                bg.a = nk_propertyf(context, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(context);
            }

        }
        nk_end(context);
        */


        camera_update(&cam);

        /* pre-rendering setup */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* draw skybox */
        glDepthMask(GL_FALSE);
        shader_use(&skybox_shader);
        mesh_bind(skybox);
        cubemap_texture_bind(cubemap);
        /* remove translation */
        mat4 cam_no_translation;
        mnf_mat4_copy(cam.view, cam_no_translation);
        cam_no_translation[3][0] = 0;
        cam_no_translation[3][1] = 0;
        cam_no_translation[3][2] = 0;
        cam_no_translation[3][3] = 1;
        //shader_uniform_mat4(skybox_shader, "u_model", skybox.model);
        shader_uniform_mat4(skybox_shader, "u_view", cam_no_translation);
        shader_uniform_mat4(skybox_shader, "u_projection", cam.projection);
        mesh_draw(skybox);
        glDepthMask(GL_TRUE);


        world_grid_draw(cube, color_shader, default_texture, cam);

        /* drawing light source */
        shader_use(&light_shader);
        mesh_bind(light_source);
        texture_bind(default_texture);
        vec3 light_pos = {5.0, 10.0, -8.0};
        mesh_set_scale(&light_source, (vec3){0.2, 0.2, 0.2});
        mesh_set_position(&light_source, light_pos);
        mesh_update_transform(&light_source);
        shader_uniform_mat4(light_shader, "u_model", light_source.model);
        shader_uniform_mat4(light_shader, "u_view", cam.view);
        shader_uniform_mat4(light_shader, "u_projection", cam.projection);
        mesh_draw(light_source);

        /* drawing lit object */
        shader_use(&lit_shader);
        mesh_bind(cube);
        texture_bind(default_texture);
        vec3 obj_pos = {1.0, 0.5, -0.6};
        mesh_set_scale(&cube, (vec3){5, 1, 7});
        mesh_set_position(&cube, obj_pos);
        shader_uniform_vec3(lit_shader, "u_material.ambient", (vec3){1.0, 1.0, 1.0});
        shader_uniform_vec3(lit_shader, "u_material.diffuse", (vec3){1.0, 1.0, 1.0});
        shader_uniform_vec3(lit_shader, "u_material.specular", (vec3){0.5, 0.5, 0.5});
        shader_uniform_float(lit_shader, "u_material.shininess", 16.0);
        shader_uniform_vec3(lit_shader, "u_light.pos", light_pos);
        shader_uniform_vec3(lit_shader, "u_light.ambient", (vec3){0.2, 0.2, 0.2});
        shader_uniform_vec3(lit_shader, "u_light.diffuse", (vec3){0.5, 0.5, 0.5});
        shader_uniform_vec3(lit_shader, "u_light.specular", (vec3){1.0, 1.0, 1.0});

        shader_uniform_vec3(lit_shader, "u_view_pos", cam.pos);
        shader_uniform_mat4(lit_shader, "u_model", cube.model);
        shader_uniform_mat4(lit_shader, "u_view", cam.view);
        shader_uniform_mat4(lit_shader, "u_projection", cam.projection);
        mesh_update_transform(&cube);
        mesh_draw(cube);

        //nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(window);
    }

    mesh_destroy(&cube);
    mesh_destroy(&light_source);
    shader_destroy(&basic_shader);

    //nk_glfw3_shutdown();
    glfwTerminate();
    window = NULL;

    return 0;
}
