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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <glad/gl.h>

#include "camera.h"
#include "shader.h"
#include "geometry.h"
#include "texture.h"
#include "mesh.h"
#include "scene.h"
#include "material.h"
#include "logger.h"
#include "platform.h"
#include "darray.h"
#include "assert.h"

struct scene scene = {0};
struct camera cam = {0};

bool SAGE_DRAW_SKYBOX = false;

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
    shader_uniform_1i(shader, "u_texture", 0);
    texture_bind(texture, 0);
    mesh_bind(mesh);

    /* x-axis */
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){200.0, 0.01, 0.01});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){0.8, 0.20, 0.4, 1.0});
    mesh_draw(mesh);

    /* y-axis */
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){0.01, 200.0, 0.01});
    mesh_set_rotation(&mesh, (vec3){0, MNF_RAD(90), 0});
    //mesh_set_position(&mesh, (vec3){0.0, 100.0, 0.0});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){0.5, 0.7, 0.25, 1.0});
    mesh_draw(mesh);
    /* z-axis */
    mesh_reset_transform(&mesh);
    mesh_set_scale(&mesh, (vec3){0.01, 0.01, 200.0});
    mesh_set_rotation(&mesh, (vec3){0, 0, MNF_RAD(90)});
    mesh_update_transform(&mesh);
    shader_uniform_mat4(shader, "u_model", mesh.model);
    shader_uniform_mat4(shader, "u_view", cam.view);
    shader_uniform_mat4(shader, "u_projection", cam.projection);
    shader_uniform_vec4(shader, "u_color", (vec4){0.3, 0.5, 0.7, 1.0});
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

void process_input(struct platform *platform, double dt)
{
    struct input_state *input = platform->input;
    bool *keys = input->keys;
    bool *mouse_buttons = input->mouse_buttons;

    if (keys[KEY_ESC])
        platform->running = false;

    if (keys[KEY_W])
        camera_move(&cam, MOVE_FORWARD, dt);

    if (keys[KEY_A])
        camera_move(&cam, STRAFE_LEFT, dt);

    if (keys[KEY_S])
        camera_move(&cam, MOVE_BACKWARD, dt);

    if (keys[KEY_D])
        camera_move(&cam, STRAFE_RIGHT, dt);

    if (keys[KEY_SPACE])
        camera_move(&cam, MOVE_UP, dt);

    if (keys[KEY_LCTRL])
        camera_move(&cam, MOVE_DOWN, dt);

    if (keys[KEY_1])
        SAGE_DRAW_SKYBOX = true;

    if (keys[KEY_2])
        SAGE_DRAW_SKYBOX = false;

    if (keys[KEY_3])
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (keys[KEY_4])
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (mouse_buttons[MOUSE_RIGHT])
        cam.can_move = true;
    else
        cam.can_move = false;

    camera_mouse(&cam, input->mouse_dx, input->mouse_dy);

    /* POSSIBLE BUG: i don't think the ray works (haven't tested though)
    if (mouse_buttons[MOUSE_LEFT]) {
        vec3 ray;
        ray_cast(input->mouse_x,
                 input->mouse_y,
                 platform->viewport_width,
                 platform->viewport_height,
                 cam.projection,
                 cam.view,
                 ray);
        SDEBUG("click in world pos, (%f, %f, %f)", ray[0], ray[1], ray[2]);
    }
    */
    /* get first object hit by ray */
    /* for node in scene */
    /* if ray intersects with node */
    /* select object & break */
}

int main(int argc, char **argv)
{
    if (argc > 1) {
        SFATAL("Sage has no flags, run the binary by itself.");
        exit(1);
    }

    struct platform platform = {0};
    platform_window_init(&platform, 640, 480, 640, 480);

    /* setup gl parameters */
    glViewport(0, 0, platform.viewport_width, platform.viewport_height);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    float aspect = (float) platform.viewport_width / (float) platform.viewport_height;
    camera_init(&cam, CAM_DEFAULT_POS, CAM_DEFAULT_FORWARD, CAM_DEFAULT_UP);
    camera_perspective(
        &cam,
        FOV_DEFAULT, 
        aspect,
        PERSPECTIVE_DEFAULT_NEAR, 
        PERSPECTIVE_DEFAULT_FAR
    );

    struct shader shaders[16] = {0};
    shaders[SHADER_BASIC] = shader_create("res/shaders/basic.glsl");
    shaders[SHADER_COLOR] = shader_create("res/shaders/color.glsl");
    shaders[SHADER_SKYBOX] = shader_create("res/shaders/skybox.glsl");
    shaders[SHADER_LIGHT] = shader_create("res/shaders/light.glsl");
    shaders[SHADER_PHONG] = shader_create("res/shaders/phong.glsl");
    shaders[SHADER_GOURAD] = shader_create("res/shaders/gourad.glsl");

    struct material plastic_material = {
        .shininess = 100
    };

    struct light light = {
        .type = LIGHT_POINT,
        .pos = {1.5, 2.7, -2.3},
        .diffuse = {1, 1, 1},
        .specular = {1.0, 1.0, 1.0}
    };

    struct light light_sun = {
        .type = LIGHT_DIRECTIONAL,
        .pos = {25, -30, 25},
        .diffuse = {1.0, 1.0, 1.0},
        .specular = {0.8, 0.9, 0.8}
    };

    struct mesh cube = mesh_create(CUBE_VERTEX_ARRAY,
                                   sizeof(CUBE_VERTEX_ARRAY));
    struct mesh light_source = mesh_create(CUBE_VERTEX_ARRAY,
                                   sizeof(CUBE_VERTEX_ARRAY));
    struct mesh skybox = mesh_create(CUBE_VERTEX_ARRAY,
                                     sizeof(CUBE_VERTEX_ARRAY));

    struct texture base_texture = texture_create("res/textures/base.png", TEXTURE_DIFFUSE);
    struct texture container_diffuse = texture_create("res/textures/container-diffuse.png", TEXTURE_DIFFUSE);
    struct texture container_specular = texture_create("res/textures/container-specular.png", TEXTURE_SPECULAR);
    struct texture uv_grid_texture = texture_create("res/textures/uv-grid.jpg", TEXTURE_DIFFUSE);
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

    scene.cam = &cam;

    double previous_seconds = platform_get_time();

    /* Configure shaders using uniform samplers to tell OpenGL which texture
     * unit will take which slot. This only needs to be done once. */
    shader_use(shaders[SHADER_PHONG]);
    shader_uniform_1i(shaders[SHADER_PHONG], "u_material.diffuse", 0);
    shader_uniform_1i(shaders[SHADER_PHONG], "u_material.specular", 1);

    mnf_vec3_copy((vec3) {0.4, 0.4, 0.4}, scene.ambient);

    /* render loop */
    while (!platform_should_close(&platform)) {
        double current_seconds = platform_get_time();
        double dt = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        platform_poll_input(&platform);
        process_input(&platform, dt);

        scene_render(&scene);

        if (SAGE_DRAW_SKYBOX) skybox_draw(shaders[SHADER_SKYBOX], skybox, cubemap, cam.view, cam.projection);
        scene_world_grid_draw(cam, cube, shaders[SHADER_COLOR], default_texture);

        /* drawing light source */
        shader_use(shaders[SHADER_LIGHT]);
        mesh_bind(light_source);
        texture_bind(default_texture, 0);

        mesh_set_scale(&light_source, (vec3){0.2, 0.2, 0.2});
        mnf_vec3_copy((vec3){
            cos(current_seconds) * 6 + 2,
            sin(current_seconds) *8.0,
            sin(current_seconds) * cos(current_seconds) * 16}, light.pos);
        mesh_set_position(&light_source, light.pos);
        mesh_update_transform(&light_source);

        shader_uniform_mat4(shaders[SHADER_LIGHT], "u_model", light_source.model);
        shader_uniform_mat4(shaders[SHADER_LIGHT], "u_view", cam.view);
        shader_uniform_mat4(shaders[SHADER_LIGHT], "u_projection", cam.projection);
        mesh_draw(light_source);

        /* draw lit cube */
        shader_use(shaders[SHADER_PHONG]);
        mesh_bind(cube);
        //mesh_reset_transform(&cube);
        texture_bind(container_diffuse, 0);
        texture_bind(container_specular, 1);

        vec3 obj_pos = {4.0, 0.5, -8};
        mesh_set_scale(&cube, (vec3){2, 2, 2});
        //mesh_set_rotation(&cube, (vec3){cos(current_seconds), sin(current_seconds), 0.0});
        mesh_set_position(&cube, obj_pos);

        lighting_model_set_params(scene.ambient,
                                  light,
                                  plastic_material,
                                  shaders[SHADER_PHONG]);

        shader_uniform_vec3(shaders[SHADER_PHONG], "u_view_pos", cam.pos);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_model", cube.model);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_view", cam.view);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_projection", cam.projection);

        mesh_update_transform(&cube);
        mesh_draw(cube);
        
        /* BUG. THIS SHOULD NOT BE AFFECTING THE CUBE AFTER DRAW CALL YET IT DOES */
        //mesh_reset_transform(&cube);
        /* IT IS ALSO PREVENTING THE ABOVE TRANSFORMATIONS FROM WORKING */

        /* drawing this one also swaps the above cube's texture with this one */
        mesh_bind(cube);
        texture_bind(base_texture, 0);
        texture_bind(default_texture, 1);
        vec3 obj2_pos = {6.0, -4, -6};
        mesh_set_scale(&cube, (vec3){10, 2, 10});
        //mesh_set_rotation(&cube, (vec3){cos(current_seconds), sin(current_seconds), 0.0});
        mesh_set_position(&cube, obj2_pos);

        lighting_model_set_params(scene.ambient,
                                  light,
                                  plastic_material,
                                  shaders[SHADER_PHONG]);

        shader_uniform_vec3(shaders[SHADER_PHONG], "u_view_pos", cam.pos);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_model", cube.model);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_view", cam.view);
        shader_uniform_mat4(shaders[SHADER_PHONG], "u_projection", cam.projection);

        mesh_update_transform(&cube);
        mesh_draw(cube);

        platform_swap_buffer(&platform);
    }

    platform_window_shutdown(&platform);
    /* TODO: clean up resources */
    mesh_destroy(&cube);
    mesh_destroy(&skybox);
    mesh_destroy(&light_source);

    return 0;
}
