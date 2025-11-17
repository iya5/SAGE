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

typedef struct nk_context context;

static void transform_model_pos_widget(context *self, struct model *model);
static void transform_model_rotation_widget(context *self, struct model *model);

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
void ui_draw_frame(struct ui *ui, struct scene *scene, struct platform *platform)
{
    float width = platform->viewport_width;
    float height = platform->viewport_height;

    struct nk_context *context = ui->context;

    nk_glfw3_new_frame();

    if (nk_begin(context, "Sage 3D Renderer", nk_rect(0, 0, width, 10), NK_WINDOW_TITLE)) {}
    nk_end(context);

    if (nk_begin(context, "Nuklear window", nk_rect(50, 50, 230, 500),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE 
                 | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)) {


        enum {EASY, HARD};
        static int op = EASY;

        nk_layout_row_static(context, 30, 80, 1);
        if (nk_button_label(context, "button"))
            SDEBUG("button pressed");

        nk_layout_row_dynamic(context, 30, 2);
        if (nk_option_label(context, "easy", op == EASY)) op = EASY;
        if (nk_option_label(context, "hard", op == HARD)) op = HARD;

        // transform
        for (uint32_t i = 0; i < scene->models->len; i++) {
            struct model *model = darray_at(scene->models, i);
            nk_layout_row_dynamic(context, 20, 1);
            nk_label(context, model->name, NK_TEXT_LEFT);
            transform_model_pos_widget(context, model);
            transform_model_rotation_widget(context, model);
        }

        nk_layout_row_dynamic(context, 20, 1);
        nk_label(context, "background:", NK_TEXT_LEFT);
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

static void transform_model_pos_widget(context *self, struct model *model)
{
    struct transform *transform= &model->transform;
    float transform_step = 0.2;
    float x_pos = transform->position[0];
    float y_pos = transform->position[1];
    float z_pos = transform->position[2];

    /* x-axis position */
    nk_layout_row_begin(self, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(self, 5);
        nk_label(self, "X", NK_TEXT_LEFT);
        nk_layout_row_push(self, 200);
        nk_slider_float(self, -10, &x_pos, 10, transform_step);
    }
    nk_layout_row_end(self);

    /* y-axis position */
    nk_layout_row_begin(self, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(self, 5);
        nk_label(self, "Y", NK_TEXT_LEFT);
        nk_layout_row_push(self, 200);
        nk_slider_float(self, -10, &y_pos, 10, transform_step);
    }
    nk_layout_row_end(self);

    /* z-axis position */
    nk_layout_row_begin(self, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(self, 5);
        nk_label(self, "Z", NK_TEXT_LEFT);
        nk_layout_row_push(self, 200);
        nk_slider_float(self, -10, &z_pos, 10, transform_step);
    }
    nk_layout_row_end(self);

    transform->position[0] = x_pos;
    transform->position[1] = y_pos;
    transform->position[2] = z_pos;

    model_translate(model, transform->position);
}


static void transform_model_rotation_widget(context *self, struct model *model)
{
    struct transform *transform= &model->transform;
    float transform_step = 1;
    float x_deg = MNF_DEG(transform->rotation[0]);
    float y_deg = MNF_DEG(transform->rotation[1]);
    float z_deg = MNF_DEG(transform->rotation[2]);

    /* x-axis position */
    nk_layout_row_begin(self, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(self, 5);
        nk_label(self, "X", NK_TEXT_LEFT);
        nk_layout_row_push(self, 200);
        nk_slider_float(self, -360, &x_deg, 360, transform_step);
    }
    nk_layout_row_end(self);

    /* y-axis position */
    nk_layout_row_begin(self, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(self, 5);
        nk_label(self, "Y", NK_TEXT_LEFT);
        nk_layout_row_push(self, 200);
        nk_slider_float(self, -360, &y_deg, 360, transform_step);
    }
    nk_layout_row_end(self);

    /* z-axis position */
    nk_layout_row_begin(self, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(self, 5);
        nk_label(self, "Z", NK_TEXT_LEFT);
        nk_layout_row_push(self, 200);
        nk_slider_float(self, -360, &z_deg, 360, transform_step);
    }
    nk_layout_row_end(self);

    transform->rotation[0] = MNF_RAD(x_deg);
    transform->rotation[1] = MNF_RAD(y_deg);
    transform->rotation[2] = MNF_RAD(z_deg);
    model_rotation(model, transform->rotation);
}
