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
#include "mesh.h"
#include "model.h"
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
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#include <nuklear.h>
#include <demo/glfw_opengl4/nuklear_glfw_gl4.h>

#include "ui.h"
#include "logger.h"
#include "scene.h"

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

/* drawing windows starts at (0, 0) at the top left */
void ui_draw_frame(struct ui *ui, struct scene *scene)
{
    struct nk_context *context = ui->context;

    nk_glfw3_new_frame();

    if (nk_begin(context, "Nuklear window", nk_rect(100, 0, 500, 500),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)) {

        struct model *model = darray_at(scene->models, 0);
        struct transform *transform = &model->transform;

        enum {EASY, HARD};
        static int op = EASY;
        static int property = 0;


        nk_layout_row_static(context, 30, 80, 1);
        if (nk_button_label(context, "button"))
            SDEBUG("button pressed");

        nk_layout_row_dynamic(context, 30, 2);
        if (nk_option_label(context, "easy", op == EASY)) op = EASY;
        if (nk_option_label(context, "hard", op == HARD)) op = HARD;

        nk_layout_row_dynamic(context, 25, 1);
        nk_property_float(context, "X::", 0, &transform->position[0], 100, 10, 1);

        nk_layout_row_dynamic(context, 25, 1);
        nk_property_float(context, "Y:", 0, &transform->position[1], 100, 10, 1);

        nk_layout_row_dynamic(context, 25, 1);
        nk_property_float(context, "2:", 0, &transform->position[2], 100, 10, 1);

        model_translate(model, transform->position);

        nk_layout_row_dynamic(context, 20, 1);
        nk_label(context, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(context, 25, 1);

        /*
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
            */
    }
    nk_end(context);
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

void ui_shutdown(struct ui *ui)
{
    SINFO("Shutting down IMGUI");
    nk_glfw3_shutdown();
    ui->context = NULL;
}
