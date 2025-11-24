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

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>
#include <demo/glfw_opengl4/nuklear_glfw_gl4.h>

#include "ui_scene_graph.h"
#include "../mnf/mnf_vector.h"
#include "../mnf/mnf_util.h"

static void model_inspector(struct nk_context *ctx, struct model *model);
static void environment_light_inspector(struct nk_context *ctx, struct directional_light *light);
static void point_light_inspector(struct nk_context *ctx, struct point_light *light);
static void camera_inspector(struct nk_context *ctx, struct camera *cam);

static void ui_vec3_editor_base(struct nk_context *ctx,
                                vec3 v,
                                float min,
                                float max,
                                float step_btn,
                                float step_drag,
                                const char *label_x,
                                const char *label_y,
                                const char *label_z)
{

    nk_layout_row_dynamic(ctx, 25, 3);
    nk_property_float(ctx, label_x, min, &v[0], max, step_btn, step_drag);
    nk_property_float(ctx, label_y, min, &v[1], max, step_btn, step_drag);
    nk_property_float(ctx, label_z, min, &v[2], max, step_btn, step_drag);
}


static void ui_vec3_editor_rgb(struct nk_context *ctx,
                               vec3 v,
                               float min,
                               float max,
                               float step_btn,
                               float step_drag)
{
    vec3 rgb;
    mnf_vec3_scale(v, 255.0f, rgb);
    ui_vec3_editor_base(ctx, rgb, min, max, step_btn, step_drag, "#R:", "#G:", "#B:");
    mnf_vec3_scale(rgb, 1.0f / 255.0f, v);
}


static void ui_vec3_editor_xyz(struct nk_context *ctx,
                               vec3 v,
                               float min,
                               float max,
                               float step_btn,
                               float step_drag)
{
    ui_vec3_editor_base(ctx, v, min, max, step_btn, step_drag, "#X:", "#Y:", "#Z:");
}


static void ui_vec3_editor_deg(struct nk_context *ctx,
                               vec3 v,
                               float min,
                               float max,
                               float step_btn,
                               float step_drag)
{
    vec3 deg;
    mnf_vec3_scale(v, 180.0f / PI, deg);
    ui_vec3_editor_base(ctx, deg, min, max, step_btn, step_drag, "#X:", "#Y:", "#Z:");
    mnf_vec3_scale(deg, PI / 180.0f, v);
}

void ui_draw_inspector(struct nk_context *ctx, struct ui_scene_node *node)
{
    if (nk_begin(ctx, "Inspector", nk_rect(10, 500, 300, 300),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_MAXIMIZED)) {
        if (node == NULL) goto end;

        switch (node->type) {
            case (UI_NODE_MODEL): model_inspector(ctx, node->data); break;
            case (UI_NODE_POINT_LIGHT): point_light_inspector(ctx, node->data); break;
            case (UI_NODE_ENVIRONMENT_LIGHT): environment_light_inspector(ctx, node->data); break;
            case (UI_NODE_CAMERA): camera_inspector(ctx, node->data); break;
        }
    }

end:
    nk_end(ctx);
}

static void model_inspector(struct nk_context *ctx, struct model *model)
{
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, model->name, NK_TEXT_LEFT);
    if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {
        nk_label(ctx, "Position", NK_TEXT_LEFT);
        ui_vec3_editor_xyz(ctx, model->transform.position, -UINT16_MAX, UINT16_MAX, 1, 0.01);
        nk_label(ctx, "Rotation", NK_TEXT_LEFT);
        ui_vec3_editor_deg(ctx, model->transform.rotation, -UINT16_MAX, UINT16_MAX, 10, 0.1);
        nk_label(ctx, "Scale", NK_TEXT_LEFT);
        ui_vec3_editor_xyz(ctx, model->transform.scale, -UINT16_MAX, UINT16_MAX, 0.1, 1);
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

static void environment_light_inspector(struct nk_context *ctx, struct directional_light *light)
{
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Environment Light", NK_TEXT_LEFT);

    nk_label(ctx, "Direction:", NK_TEXT_LEFT);
    ui_vec3_editor_xyz(ctx, light->direction, -UINT16_MAX, UINT16_MAX, 10, 0.05);

    nk_label(ctx, "Ambient:", NK_TEXT_LEFT);
    ui_vec3_editor_rgb(ctx, light->ambient, 0.0f, 255.0f, 10.0f, 1.0f);

    nk_label(ctx, "Diffuse:", NK_TEXT_LEFT);
    ui_vec3_editor_rgb(ctx, light->diffuse, 0.0f, 255.0f, 10.0f, 1.0f);

    nk_label(ctx, "Specular:", NK_TEXT_LEFT);
    ui_vec3_editor_rgb(ctx, light->specular, 0.0f, 255.0f, 10.0f, 1.0f);
}

static void point_light_inspector(struct nk_context *ctx, struct point_light *light)
{
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, light->name, NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Visibility", NK_MINIMIZED)) {
        nk_bool visible = !light->visible;
        nk_checkbox_label(ctx, "Visible", &visible);
        light->visible = !visible;
        nk_tree_pop(ctx);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {
        nk_label(ctx, "Position", NK_TEXT_LEFT);
        ui_vec3_editor_xyz(ctx, light->pos, -UINT16_MAX, UINT16_MAX, 10, 0.05);
        /* Changing the size does not change the actual light itself, this is
           just for the geometric body & nothing else. Changing the position
           however, internally, is done by using the actual pos member of the
           light struct which the scene uses for rendering the light and its
           geometric body, but the other attributes of the geometric body, i.e,
           scale, & rotation, have no effect on the light. */
        struct transform *transform = &light->geometric_model.transform;
        nk_label(ctx, "Scale", NK_TEXT_LEFT);
        ui_vec3_editor_xyz(ctx, transform->scale, -UINT16_MAX, UINT16_MAX, 10, 0.1);
        nk_tree_pop(ctx);
    }

    nk_label(ctx, "Visual Color:", NK_TEXT_LEFT);
    ui_vec3_editor_rgb(ctx, light->color, 0.0f, 255.0f, 10.0f, 1.0f);

    nk_label(ctx, "Diffuse:", NK_TEXT_LEFT);
    ui_vec3_editor_rgb(ctx, light->diffuse, 0.0f, 255.0f, 10.0f, 1.0f);

    nk_label(ctx, "Specular:", NK_TEXT_LEFT);
    ui_vec3_editor_rgb(ctx, light->specular, 0.0f, 255.0f, 10.0f, 1.0f);
}

static void camera_inspector(struct nk_context *ctx, struct camera *cam)
{

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Camera", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_property_float(ctx, "FOV:", 0.1, &cam->fov, 179.99, 1, 0.2);
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_property_float(ctx, "speed:", 0.0, &cam->speed, UINT16_MAX, 1, 1);
}

