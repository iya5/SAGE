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
#include "ui_scene_graph.h"
#include <stdbool.h>
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

#include "ui.h"
#include "ui_util.h"
#include "../darray.h"
#include "../logger.h"

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
}

void ui_begin_frame(struct ui *ui, struct scene *scene, struct platform *platform)
{
    nk_glfw3_new_frame();
    struct nk_context *ctx = ui->context;

    static bool first_load = true;
    float width = platform->viewport_width;

    if (nk_begin(ctx, "Sage 3D Renderer", nk_rect(0, 0, width, 10), NK_WINDOW_TITLE)) {}
    nk_end(ctx);

    ui_rebuild_dirty_scene_graph(&ui->scene_graph, scene);
    ui_draw_scene_graph(ctx, &ui->scene_graph);
    ui_draw_inspector(ctx, ui->scene_graph.selected_node);

    if (nk_begin(ctx, "Debug", nk_rect(500, 50, 300, 300),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_MINIMIZED )) {
        char info_buffer[128];
        snprintf(info_buffer, 128, "%d fps (%.2f ms)", platform->fps, platform->frame_time * 1000);
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, info_buffer, NK_TEXT_LEFT);

        if (nk_tree_push(ctx, NK_TREE_TAB, "Lighting Parameters", NK_MINIMIZED)) {
            nk_bool ambient = scene->lighting_params.enable_ambient;
            nk_bool diffuse = scene->lighting_params.enable_diffuse;
            nk_bool specular = scene->lighting_params.enable_specular;
            nk_checkbox_label(ctx, "Ambient", &ambient);
            nk_checkbox_label(ctx, "Diffuse", &diffuse);
            nk_checkbox_label(ctx, "Specular", &specular);
            scene->lighting_params.enable_ambient = ambient;
            scene->lighting_params.enable_diffuse = diffuse;
            scene->lighting_params.enable_specular = specular;

            nk_tree_pop(ctx);
        }
        if (nk_tree_push(ctx, NK_TREE_TAB, "Scene", NK_MINIMIZED)) {
            nk_bool skybox = scene->draw_skybox;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_checkbox_label(ctx, "Enable skybox", &skybox);
            ui_vec3_editor_rgb(ctx, scene->clear_color, 0.0f, 255.0f, 1.0f, 0.1f);
            scene->draw_skybox = skybox;
            nk_label(ctx, "Clear Color", NK_TEXT_LEFT);
            ui_vec3_editor_rgb(ctx, scene->clear_color, 0.0f, 255.0f, 1.0f, 0.1f);
            nk_tree_pop(ctx);
        }
    }
    nk_end(ctx);

    if (first_load) {
        first_load = false;
        nk_window_collapse(ctx, "Debug", NK_MINIMIZED);
    }
    //ui->hovered = nk_window_is_any_hovered(context);
}

void ui_end_frame(void)
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
    struct nk_context *ctx = ui->context;

    nk_input_begin(ctx);
    struct input_state input = platform->input;
    struct mouse mouse = input.mouse;
    nk_input_scroll(ctx, nk_vec2(mouse.scroll_x, mouse.scroll_y));
    nk_input_motion(ctx, mouse.x, mouse.y);

    nk_input_end(ctx);
}

void ui_shutdown(struct ui *ui)
{
    SINFO("Shutting down IMGUI");
    darray_free(ui->scene_graph.nodes);
    nk_glfw3_shutdown();
    ui->context = NULL;
}
