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

#include "../mnf/mnf_vector.h"
#include "../mnf/mnf_util.h"

void ui_vec3_editor_base(struct nk_context *ctx,
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


void ui_vec3_editor_rgb(struct nk_context *ctx,
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


void ui_vec3_editor_xyz(struct nk_context *ctx,
                               vec3 v,
                               float min,
                               float max,
                               float step_btn,
                               float step_drag)
{
    ui_vec3_editor_base(ctx, v, min, max, step_btn, step_drag, "#X:", "#Y:", "#Z:");
}


void ui_vec3_editor_deg(struct nk_context *ctx,
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
