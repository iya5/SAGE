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

#include <stdbool.h>
/* necessary to include glfw header before nuklear */
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
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

#include "darray.h"
#include "lighting.h"
#include "mesh.h"
#include "model.h"
#include "platform.h"
#include "ui.h"
#include "mnf/mnf_vector.h"
#include "logger.h"
#include "scene.h"
#include "mnf/mnf_util.h"

#define COLOR_TO_UI(v) (v) * 255.0f
#define UI_TO_COLOR(v) (v) / 255.0f

typedef struct nk_context context;

static void transform_model_pos_widget(context *ctx, struct model *model);
static void transform_model_rotation_widget(context *ctx, struct model *model);
static void transform_model_scale_widget(context *ctx, struct model *model);

static void scene_window(context *ctx, darray *models);
static void model_inspector(context *ctx, struct model *model);
static void env_light_inspector(context *ctx, struct directional_light *light);
static void point_light_inspector(context *ctx, struct point_light *light);
static void camera_inspector(context *ctx, struct camera *cam);

static void vec3_color_to_ui(vec3 src, vec3 dest);
static void vec3_ui_to_color(vec3 src, vec3 dest);

struct nk_image geometry_icon;
struct nk_image camera_icon;
struct nk_image light_icon;
struct nk_image sun_icon;

/* Scene is a flat struct containing a darray of models & lights, a struct to a
   camera, and a struct to a direcctional light (the sun), hence two represent
   the scene in the UI with each of those components as nodes; we either
   restructure the struct to act more as a Scene Graph mimicking scene files
   like GLTF, however, another alternative to this is just to have a UI
   represenation of the scene instead that treats those components as nodes w/o
   needing to change the underlying scene struct itself. The latter was the
   solution that was chosen as it was simpler */
enum node_type {
    NODE_CAMERA,
    NODE_POINT_LIGHT,
    NODE_ENVIRONMENT_LIGHT,
    NODE_MODEL
};

struct scene_node {
    enum node_type type;
    void *data;
    const char *name;
};

static darray *scene_nodes;

void ui_init(struct ui *ui, struct platform platform)
{
    SINFO("Initializing Nuklear Intermediate-Mode GUI");
    /* Don't allow nuklear to hog the input stream from GLFW by setting the
       callback enum to 0 instead of NK_GLFW3_INSTALL_CALLBACKS */
    ui->context = nk_glfw3_init(platform.context, 
                                0,
                                MAX_VERTEX_BUFFER,
                                MAX_ELEMENT_BUFFER);

    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    nk_glfw3_font_stash_end();

    struct texture tex = texture_create("res/icons/geometry.png");
    geometry_icon = nk_image_id(tex.id);
    tex = texture_create("res/icons/camera.png");
    camera_icon = nk_image_id(tex.id);
    tex = texture_create("res/icons/sun.png");
    sun_icon = nk_image_id(tex.id);
    tex = texture_create("res/icons/light.png");
    light_icon = nk_image_id(tex.id);
    scene_nodes = darray_alloc(sizeof(struct scene_node), 64);
}

void ui_scene_nodes_build(struct scene *scene)
{
    SINFO("Building scene nodes to the UI");
    struct scene_node camera_node = {
        .type = NODE_CAMERA,
        .data = &scene->cam,
        .name = "Camera"
    };
    darray_push(scene_nodes, &camera_node);
    struct scene_node environment_light_node = {
        .type = NODE_ENVIRONMENT_LIGHT,
        .data = &scene->environment_light,
        .name = "Environment Light"
    };
    darray_push (scene_nodes, &environment_light_node);

    for (size_t i = 0; i < scene->point_lights->len; i++) {
        struct point_light *light = darray_at(scene->point_lights, i);
        struct scene_node light_node = {
            .type = NODE_POINT_LIGHT,
            .data = light,
            .name = light->name
        };
        darray_push(scene_nodes, &light_node);
    };

    for (size_t i = 0; i < scene->models->len; i++) {
        struct model *model = darray_at(scene->models, i);
        struct scene_node model_node = {
            .type = NODE_MODEL,
            .data = model,
            .name = model->name
        };
        darray_push(scene_nodes, &model_node);
    };
}

void ui_draw(struct ui *ui, struct scene *scene, struct platform *platform)
{
    nk_glfw3_new_frame();
    struct nk_context *context = ui->context;

    static bool first_load = true;
    float width = platform->viewport_width;
    //float height = platform->viewport_height;

    if (nk_begin(context, "Sage 3D Renderer", nk_rect(0, 0, width, 10), NK_WINDOW_TITLE)) {}
    nk_end(context);

    scene_window(context, scene->models);

    if (nk_begin(context, "Debug", nk_rect(500, 50, 300, 300),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_MINIMIZED )) {
        if (nk_tree_push(context, NK_TREE_TAB, "Skybox", NK_MINIMIZED)) {
            nk_layout_row_dynamic(context, 25, 2);
            if (nk_option_label(context, "enable", scene->draw_skybox))
                scene->draw_skybox= true;
            if (nk_option_label(context, "disable", !scene->draw_skybox))
                scene->draw_skybox= false;
            nk_tree_pop(context);
        }
        char info_buffer[128];
        snprintf(info_buffer, 128, "Metrics: %d fps (%.2f ms)", platform->fps, platform->frame_time * 1000);
        nk_layout_row_dynamic(context, 25, 1);
        nk_label(context, info_buffer, NK_TEXT_LEFT);
    }
    nk_end(context);

    if (first_load) {
        first_load = false;
        nk_window_collapse(context, "Debug", NK_MINIMIZED);
    }
    ui->hovered = nk_window_is_any_hovered(context);
}

void ui_render(void)
{
    /* from glfw_opengl4/main.c nuklear demo:
       IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
       with blending, scissor, face culling, depth test and viewport and
       defaults everything back into a default state.
       Make sure to either a.) save and restore or b.) reset your own state after
       rendering the UI. */
    nk_glfw3_render(NK_ANTI_ALIASING_ON);
}

void ui_process_input(struct ui *ui, struct platform *platform)
{
    context *context = ui->context;

    nk_input_begin(context);
    struct input_state input = platform->input;
    struct mouse mouse = input.mouse;
    nk_input_scroll(context, nk_vec2(mouse.scroll_x, mouse.scroll_y));
    nk_input_motion(context, mouse.x, mouse.y);

    nk_input_end(context);
}

void ui_shutdown(struct ui *ui)
{
    SINFO("Shutting down IMGUI");
    darray_free(scene_nodes);
    nk_glfw3_shutdown();
    ui->context = NULL;
}

static void scene_window(context *ctx, darray *models)
{
    static size_t selected_index = UINT16_MAX;

    if (nk_begin(ctx, "Scene Graph", nk_rect(10, 50, 300, 400),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_MAXIMIZED)) {
        if (nk_tree_push(ctx, NK_TREE_NODE, "Scene", NK_MAXIMIZED)) {
            for (size_t i = 0; i < scene_nodes->len; i++) {
                struct scene_node *node = darray_at(scene_nodes, i);
                nk_bool is_selected = (i == selected_index);

                nk_layout_row_template_begin(ctx, 20);
                nk_layout_row_template_push_static(ctx, 20);
                nk_layout_row_template_push_dynamic(ctx);
                nk_layout_row_template_end(ctx);

                struct nk_image icon;
                switch (node->type) {
                    case (NODE_MODEL):
                        icon = geometry_icon;
                        break;
                    case (NODE_POINT_LIGHT):
                        icon = light_icon;
                        break;
                    case (NODE_ENVIRONMENT_LIGHT):
                        icon = sun_icon;
                        break;
                    case (NODE_CAMERA):
                        icon = camera_icon;
                        break;
                }

                nk_image(ctx, icon);
                if (nk_selectable_label(ctx, node->name, NK_LEFT, &is_selected))
                    selected_index = i;
            }
            nk_tree_pop(ctx);
        }
    }
    nk_end(ctx);

    if (nk_begin(ctx, "Inspector", nk_rect(10, 500, 300, 300),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_MAXIMIZED)) {
        if (selected_index != UINT16_MAX) {
            struct scene_node *node = darray_at(scene_nodes, selected_index);

            switch (node->type) {
                case (NODE_MODEL):
                    model_inspector(ctx, node->data);
                    break;
                case (NODE_POINT_LIGHT):
                    point_light_inspector(ctx, node->data);
                    break;
                case (NODE_ENVIRONMENT_LIGHT):
                    env_light_inspector(ctx, node->data);
                    break;
                case (NODE_CAMERA):
                    camera_inspector(ctx, node->data);
                    break;
            }
        }
    }
    nk_end(ctx);
}

static void model_inspector(context *ctx, struct model *model)
{
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, model->name, NK_TEXT_LEFT);
    if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {
        transform_model_pos_widget(ctx, model);
        transform_model_rotation_widget(ctx, model);
        transform_model_scale_widget(ctx, model);
        nk_tree_pop(ctx);
    }
    if (nk_tree_push(ctx, NK_TREE_TAB, "Visibility", NK_MINIMIZED)) {
        nk_bool visible = !model->visible;
        nk_checkbox_label(ctx, "Visible", &visible);
        model->visible = !visible;
        nk_tree_pop(ctx);
    }
    if (nk_tree_push(ctx, NK_TREE_TAB, "Material", NK_MINIMIZED)) {
        nk_property_float(ctx, "#Shininess:", 1, &model->material.shininess, 2048, 1, 1);
        nk_tree_pop(ctx);
    }
}

static void env_light_inspector(context *ctx, struct directional_light *light)
{
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Environment Light", NK_TEXT_LEFT);

    static const float step = 1;
    static const float dir_step = 0.5;

    nk_label(ctx, "Direction:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#X:", -UINT16_MAX, &light->direction[0], UINT16_MAX, 10, dir_step);
    nk_property_float(ctx, "#Y:", -UINT16_MAX, &light->direction[1], UINT16_MAX, 10, dir_step);
    nk_property_float(ctx, "#Z:", -UINT16_MAX, &light->direction[2], UINT16_MAX, 10, dir_step);

    vec3 ambient;
    vec3_color_to_ui(light->ambient, ambient);
    nk_label(ctx, "Ambient:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#R:", 0, &ambient[0], 255, 10, step);
    nk_property_float(ctx, "#G:", 0, &ambient[1], 255, 10, step);
    nk_property_float(ctx, "#B:", 0, &ambient[2], 255, 10, step);
    vec3_ui_to_color(ambient, light->ambient);

    vec3 diffuse;
    vec3_color_to_ui(light->diffuse, diffuse);
    nk_label(ctx, "Diffuse:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#R:", 0, &diffuse[0], 255, 10, step);
    nk_property_float(ctx, "#G:", 0, &diffuse[1], 255, 10, step);
    nk_property_float(ctx, "#B:", 0, &diffuse[2], 255, 10, step);
    vec3_ui_to_color(diffuse, light->diffuse);

    vec3 specular;
    vec3_color_to_ui(light->specular, specular);
    nk_label(ctx, "Specular:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#R:", 0, &specular[0], 255, 10, step);
    nk_property_float(ctx, "#G:", 0, &specular[1], 255, 10, step);
    nk_property_float(ctx, "#B:", 0, &specular[2], 255, 10, step);
    vec3_ui_to_color(specular, light->specular);
}

static void point_light_inspector(context *ctx, struct point_light *light)
{
    static const float pos_step = 0.05;
    static const float scale_step = 0.05;
    static const float step = 1;

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, light->name, NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Visibility", NK_MINIMIZED)) {
        nk_bool visible = !light->visible;
        nk_checkbox_label(ctx, "Visible", &visible);
        light->visible = !visible;
        nk_tree_pop(ctx);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {
        nk_label(ctx, "Position:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 3);
        nk_property_float(ctx, "#X:", -UINT16_MAX, &light->pos[0], UINT16_MAX, 10, pos_step);
        nk_property_float(ctx, "#Y:", -UINT16_MAX, &light->pos[1], UINT16_MAX, 10, pos_step);
        nk_property_float(ctx, "#Z:", -UINT16_MAX, &light->pos[2], UINT16_MAX, 10, pos_step);

        /* Changing the size does not change the actual light itself, this is
           just for the geometric body & nothing else. Changing the position
           however, internally, is done by using the actual pos member of the
           light struct which the scene uses for rendering the light and its
           geometric body, but the other attributes of the geometric body, i.e,
           scale, & rotation, have no effect on the light. */
        struct transform *transform = &light->geometric_model.transform;
        nk_label(ctx, "Scale:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 3);
        nk_property_float(ctx, "#X:", -UINT16_MAX, &transform->scale[0], UINT16_MAX, 10, scale_step);
        nk_property_float(ctx, "#Y:", -UINT16_MAX, &transform->scale[1], UINT16_MAX, 10, scale_step);
        nk_property_float(ctx, "#Z:", -UINT16_MAX, &transform->scale[2], UINT16_MAX, 10, scale_step);
        nk_tree_pop(ctx);
    }

    vec3 color;
    vec3_color_to_ui(light->color, color);
    nk_label(ctx, "Visual Color:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#R:", 0, &color[0], 255, 10, step);
    nk_property_float(ctx, "#G:", 0, &color[1], 255, 10, step);
    nk_property_float(ctx, "#B:", 0, &color[2], 255, 10, step);
    vec3_ui_to_color(color, light->color);

    vec3 diffuse;
    vec3_color_to_ui(light->diffuse, diffuse);
    nk_label(ctx, "Diffuse:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#R:", 0, &diffuse[0], 255, 10, step);
    nk_property_float(ctx, "#G:", 0, &diffuse[1], 255, 10, step);
    nk_property_float(ctx, "#B:", 0, &diffuse[2], 255, 10, step);
    vec3_ui_to_color(diffuse, light->diffuse);

    vec3 specular;
    vec3_color_to_ui(light->specular, specular);
    nk_label(ctx, "Specular:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#R:", 0, &specular[0], 255, 10, step);
    nk_property_float(ctx, "#G:", 0, &specular[1], 255, 10, step);
    nk_property_float(ctx, "#B:", 0, &specular[2], 255, 10, step);
    vec3_ui_to_color(specular, light->specular);
}

static void camera_inspector(context *ctx, struct camera *cam)
{

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Camera", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_property_float(ctx, "FOV:", 0.1, &cam->fov, 179.99, 1, 0.2);
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_property_float(ctx, "speed:", 0.0, &cam->speed, UINT16_MAX, 1, 1);
}

static void transform_model_pos_widget(context *ctx, struct model *model)
{
    struct transform *transform = &model->transform;
    static const float step = 0.05;

    nk_label(ctx, "Position", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#X:", -100, &transform->position[0], 100, 10, step);
    nk_property_float(ctx, "#Y:", -100, &transform->position[1], 100, 10, step);
    nk_property_float(ctx, "#Z:", -100, &transform->position[2], 100, 10, step);
}

static void transform_model_rotation_widget(context *ctx, struct model *model)
{
    struct transform *transform= &model->transform;
    static const float step = 1;

    float x_deg = MNF_DEG(transform->rotation[0]);
    float y_deg = MNF_DEG(transform->rotation[1]);
    float z_deg = MNF_DEG(transform->rotation[2]);

    nk_label(ctx, "Rotation", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#X:", -UINT16_MAX, &x_deg, UINT16_MAX, 10, step);
    nk_property_float(ctx, "#Y:", -UINT16_MAX, &y_deg, UINT16_MAX, 10, step);
    nk_property_float(ctx, "#Z:", -UINT16_MAX, &z_deg, UINT16_MAX, 10, step);

    transform->rotation[0] = MNF_RAD(x_deg);
    transform->rotation[1] = MNF_RAD(y_deg);
    transform->rotation[2] = MNF_RAD(z_deg);
    model_rotation(model, transform->rotation);
}

static void transform_model_scale_widget(context *ctx, struct model *model)
{
    struct transform *transform= &model->transform;
    static const float step = 0.2;

    nk_label(ctx, "Scale", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, "#X:", -UINT16_MAX, &transform->scale[0], UINT16_MAX, 10, step);
    nk_property_float(ctx, "#Y:", -UINT16_MAX, &transform->scale[1], UINT16_MAX, 10, step);
    nk_property_float(ctx, "#Z:", -UINT16_MAX, &transform->scale[2], UINT16_MAX, 10, step);
}

static void vec3_color_to_ui(vec3 src, vec3 dest)
{
    dest[0] = COLOR_TO_UI(src[0]);
    dest[1] = COLOR_TO_UI(src[1]);
    dest[2] = COLOR_TO_UI(src[2]);
}

static void vec3_ui_to_color(vec3 src, vec3 dest)
{
    dest[0] = UI_TO_COLOR(src[0]);
    dest[1] = UI_TO_COLOR(src[1]);
    dest[2] = UI_TO_COLOR(src[2]);
}
