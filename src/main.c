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

#include "mnf/mnf_matrix.h"
#include "mnf/mnf_vector.h"

#ifndef VERSION
#define VERSION "Something went wrong with VERSION"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "mnf/mnf.h" // IWYU pragma: keep
#include "slog/slog.h"
#include "config.h"
#include "camera.h"
#include "shader.h"
#include "geometry.h"
#include "texture.h"
#include "mesh.h"
#include "scene.h"
#include "material.h"
#include "light.h"

struct camera cam = {0};

void skybox_draw(struct shader skybox_shader, 
                 struct mesh skybox, 
                 struct texture cubemap,
                 mat4 view, 
                 mat4 projection)
{
    /* draw skybox */
    glDepthMask(GL_FALSE);
    shader_use(skybox_shader);
    mesh_bind(skybox);
    cubemap_texture_bind(cubemap);

    /* remove translation */
    mat4 view_no_translation;
    mnf_mat4_copy(view, view_no_translation);

    view_no_translation[3][0] = 0;
    view_no_translation[3][1] = 0;
    view_no_translation[3][2] = 0;
    view_no_translation[3][3] = 1;

    shader_uniform_mat4(skybox_shader, "u_view", view_no_translation);
    shader_uniform_mat4(skybox_shader, "u_projection", projection);
    mesh_draw(skybox);
    glDepthMask(GL_TRUE);
}

void scene_world_grid_draw(struct camera cam,
                           struct mesh mesh,
                           struct shader shader, 
                           struct texture texture)
{
    /* prolly a naive approach at drawing a world grid, could do this instead
     * in the shaders. currently implemented as drawing 3 seperate strecthed out
     * cubes representing the cardinal axis */
    shader_use(shader);
    texture_bind(texture);
    mesh_bind(mesh);

    /* x-axis */
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){200.0, 0.01, 0.01});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){1.0, 0.0, 0.0, 1.0});
    mesh_draw(mesh);

    /* y-axis */
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


    /* z-axis */
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

void ray_cast(double x_pos, 
              double y_pos, 
              int viewport_width,
              int viewport_height,
              mat4 projection,
              mat4 view,
              vec3 out)
{

    float x = (2.0 * x_pos) / viewport_width - 1.0f;
    float y = 1.0 - (2.0 * y_pos) / viewport_height;
    float z = 1.0;

    /*
     * no need to do reverse perspective division because this is a ray with
     * with no intrinsic depth. this turns the ray into 4d homogenous clip
     * coordinate space
     */
    vec4 ray = {x, y, -z, 1.0};

    /* get inverse transformations until world space */
    mat4 projection_inv;
    mat4 view_inv;
    mnf_mat4_inv(projection, projection_inv);
    mnf_mat4_inv(view, view_inv);
    mnf_mat4_mul_vec4(projection_inv, ray, ray);

    ray[2] = -1.0;  /* point forward from camera */
    ray[3] = 0.0;   /* remove translation component */

    mnf_mat4_mul_vec4(view_inv, ray, ray);
    vec3 ray_world = {ray[0], ray[1], ray[2]};
    /* normalize the ray because it's a direction vector */
    mnf_vec3_normalize(ray_world, ray_world);
    mnf_vec3_copy(ray_world, out);
}

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

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        /* cursor pos will be based on the window width and is relative to the
         * top left of the window */
        int viewport_width, viewport_height;
        glfwGetFramebufferSize(window, &viewport_width, &viewport_height);

        vec3 ray;
        ray_cast(x_pos,
                 y_pos,
                 viewport_width,
                 viewport_height,
                 cam.projection,
                 cam.view,
                 ray);
        LOG_DEBUG("click in world pos, (%f, %f, %f)", ray[0], ray[1], ray[2]);

        /* get first object hit by ray */
        /* for node in scene */
        /* if ray intersects with node */
        /* select object & break */
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
    //camera_scroll(&cam, dy);
}

void light_apply(struct light light, struct shader shader)
{
    if (light.type == LIGHT_POINT)
        shader_uniform_1i(shader, "u_flat_shading", 1);
    else
        shader_uniform_1i(shader, "u_flat_shading", 0);

    shader_uniform_vec3(shader, "u_light.pos", light.pos);
    shader_uniform_vec3(shader, "u_light.ambient", light.ambient);
    shader_uniform_vec3(shader, "u_light.diffuse", light.diffuse);
    shader_uniform_vec3(shader, "u_light.specular", light.specular);
}

void material_apply(struct material material, struct shader shader)
{
    shader_uniform_vec3(shader, "u_material.ambient", material.ambient);
    shader_uniform_vec3(shader, "u_material.diffuse", material.diffuse);
    shader_uniform_vec3(shader, "u_material.specular", material.specular);
    shader_uniform_1f(shader, "u_material.shininess", material.shininess);
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

    LOG_INFO("Loaded OpenGL %d.%d", 
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

    struct shader shaders[16] = {0};
    shaders[SHADER_BASIC] = shader_create("shaders/basic.glsl");
    shaders[SHADER_COLOR] = shader_create("shaders/color.glsl");
    shaders[SHADER_SKYBOX] = shader_create("shaders/skybox.glsl");
    shaders[SHADER_LIGHT] = shader_create("shaders/light.glsl");
    shaders[SHADER_PHONG] = shader_create("shaders/phong.glsl");
    shaders[SHADER_GOURAD] = shader_create("shaders/gourad.glsl");

    struct material plastic_material = {
        .ambient = {1.0, 1.0, 1.0},
        .diffuse = {1.0, 1.0, 1.0},
        .specular = {0.5, 0.5, 0.5},
        .shininess = 16
    };

    struct light light = {
        .type = LIGHT_POINT,
        .pos = {2.0, 4.0, -4.0},
        .ambient = {0.2, 0.2, 0.2},
        .diffuse = {0.8, 0.8, 0.8},
        .specular = {0.4, 0.4, 0.4}
    };

    struct light light_sun = {
        .type = LIGHT_DIRECTIONAL,
        .pos = {-25, 50, -25},
        .ambient = {0.5, 0.5, 0.5},
        .diffuse = {0.8, 0.8, 0.8},
        .specular = {0.0, 0.0, 0.0}
    };

    struct mesh cube = mesh_create(CUBE_VERTEX_ARRAY,
                                   sizeof(CUBE_VERTEX_ARRAY));
    struct mesh light_source = mesh_create(CUBE_VERTEX_ARRAY,
                                   sizeof(CUBE_VERTEX_ARRAY));
    struct mesh skybox = mesh_create(CUBE_VERTEX_ARRAY,
                                     sizeof(CUBE_VERTEX_ARRAY));

    struct texture base_texture = texture_create("res/textures/base.png");
    struct texture uv_grid_texture = texture_create("res/textures/uv-grid.jpg");
    struct texture default_texture = texture_create_default();

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

    /* render loop */
    while (!glfwWindowShouldClose(window)) {
        double current_seconds = glfwGetTime();
        double dt = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        glfwPollEvents();
        process_input(window, dt);

        /*
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shader_hot_reload(&basic_shader);
        */

        camera_update(&cam);

        scene_render();

        skybox_draw(shaders[SHADER_SKYBOX], skybox, cubemap, cam.view, cam.projection);
        scene_world_grid_draw(cam, cube, shaders[SHADER_COLOR], default_texture);

        /* drawing light source */
        shader_use(shaders[SHADER_LIGHT]);
        mesh_bind(light_source);
        texture_bind(default_texture);

        mesh_set_scale(&light_source, (vec3){0.2, 0.2, 0.2});
        mesh_set_position(&light_source, light.pos);
        mesh_update_transform(&light_source);

        shader_uniform_mat4(shaders[SHADER_LIGHT], "u_model", light_source.model);
        shader_uniform_mat4(shaders[SHADER_LIGHT], "u_view", cam.view);
        shader_uniform_mat4(shaders[SHADER_LIGHT], "u_projection", cam.projection);

        mesh_draw(light_source);

        /* drawing lit object */
        /* illuminating a scene only requires sum of light contributions */
        /*
         * for light (lit shader) in light sources
         *      render model in specific shader
         */

        shader_use(shaders[SHADER_PHONG]);
        mesh_bind(cube);
        texture_bind(uv_grid_texture);
        vec3 obj_pos = {1.0, 0.5, -0.6};
        mesh_set_scale(&cube, (vec3){5, 1, 7});
        mesh_set_position(&cube, obj_pos);

        light_apply(light_sun, shaders[SHADER_PHONG]);
        material_apply(plastic_material, shaders[SHADER_PHONG]);

        shader_uniform_vec3(shaders[SHADER_PHONG], "u_view_pos", cam.pos);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_model", cube.model);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_view", cam.view);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_projection", cam.projection);

        mesh_update_transform(&cube);
        mesh_draw(cube);

        glfwSwapBuffers(window);
    }

    /* TODO: clean up resources */
    mesh_destroy(&cube);
    mesh_destroy(&skybox);
    mesh_destroy(&light_source);

    glfwTerminate();
    window = NULL;

    return 0;
}
