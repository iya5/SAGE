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

/* necessary to include glfw header before nuklear */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
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
#include "../logger.h"

struct nk_image geometry_icon;
struct nk_image camera_icon;
struct nk_image light_icon;
struct nk_image sun_icon;

static void update_scene_graph(struct ui_scene_graph *scene_graph, struct scene *scene)
{
    /* camera */
    struct ui_scene_node camera = {
        .name = "Camera",
        .type = UI_NODE_CAMERA,
        .data = &scene->cam
    };
    darray_push(scene_graph->nodes, &camera);

    /* environment light */
    struct ui_scene_node environment_light = {
        .name = "Environment Light",
        .type = UI_NODE_ENVIRONMENT_LIGHT,
        .data = &scene->environment_light
    };
    darray_push(scene_graph->nodes, &environment_light);

    /* point lights */
    for (size_t i = 0; i < scene->point_lights->len; i++) {
        struct point_light *light_ptr = darray_at(scene->point_lights, i);
        struct ui_scene_node light = {
            .name = light_ptr->name,
            .type = UI_NODE_POINT_LIGHT,
            .data = light_ptr
        };
        darray_push(scene_graph->nodes, &light);
    };
    
    /* models */
    for (size_t i = 0; i < scene->models->len; i++) {
        struct model *model_ptr = darray_at(scene->models, i);
        struct ui_scene_node model = {
            .name = model_ptr->name,
            .type = UI_NODE_MODEL,
            .data = model_ptr
        };
        darray_push(scene_graph->nodes, &model);
    };

}

void ui_build_scene_graph(struct ui_scene_graph *scene_graph, struct scene *scene)
{
    static bool built = false;

    if (built) {
        SFATAL("ui_build_scene_graph() should only be called once.");
        SFATAL("For rebuilding the Scene Graph, call ui_build_scene_graph()");
        exit(1);
    }
    built = true;

    SINFO("Building Scene Graph for the UI");

    struct texture tex = texture_create("res/icons/geometry.png");
    geometry_icon = nk_image_id(tex.id);
    tex = texture_create("res/icons/camera-flipped.png");
    camera_icon = nk_image_id(tex.id);
    tex = texture_create("res/icons/sun.png");
    sun_icon = nk_image_id(tex.id);
    tex = texture_create("res/icons/light.png");
    light_icon = nk_image_id(tex.id);

    scene_graph->nodes = darray_alloc(sizeof(struct ui_scene_node), 64);
    scene_graph->dirty = false;
    scene_graph->selected_node = NULL;
    update_scene_graph(scene_graph, scene);
}

void ui_draw_scene_graph(struct nk_context *ctx, struct ui_scene_graph *scene_graph)
{
    if (nk_begin(ctx, "Scene Graph", nk_rect(10, 50, 300, 400),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_MAXIMIZED)) {
        if (nk_tree_push(ctx, NK_TREE_NODE, "Scene", NK_MAXIMIZED)) {
            for (size_t i = 0; i < scene_graph->nodes->len; i++) {
                struct ui_scene_node *node = darray_at(scene_graph->nodes, i);
                nk_bool is_selected = (node == scene_graph->selected_node);

                nk_layout_row_template_begin(ctx, 20);
                nk_layout_row_template_push_static(ctx, 20);
                nk_layout_row_template_push_dynamic(ctx);
                nk_layout_row_template_end(ctx);

                struct nk_image icon;
                switch (node->type) {
                    case (UI_NODE_MODEL): icon = geometry_icon; break;
                    case (UI_NODE_POINT_LIGHT): icon = light_icon; break;
                    case (UI_NODE_ENVIRONMENT_LIGHT): icon = sun_icon; break;
                    case (UI_NODE_CAMERA): icon = camera_icon; break;
                }

                nk_image(ctx, icon);
                if (nk_selectable_label(ctx, node->name, NK_LEFT, &is_selected))
                    scene_graph->selected_node = node;
            }
            nk_tree_pop(ctx);
        }
    }
    nk_end(ctx);
}

void ui_rebuild_dirty_scene_graph(struct ui_scene_graph *scene_graph, struct scene *scene)
{
    if (scene_graph->dirty) {
        update_scene_graph(scene_graph, scene);
        scene_graph->dirty = false;
    }
}

void ui_destroy_scene_graph(struct ui_scene_graph *scene_graph)
{
    SINFO("Destroying UI Scene Graph");
    darray_free(scene_graph->nodes);
    texture_destroy_id(&geometry_icon.handle.id);
    texture_destroy_id(&camera_icon.handle.id);
    texture_destroy_id(&light_icon.handle.id);
    texture_destroy_id(&sun_icon.handle.id);
}
