#ifndef SAGE_UI_UTIL_H
#define SAGE_UI_UTIL_H

#include "../mnf/mnf_types.h"

/* Forward  Declarations */
struct nk_context;

void ui_vec3_editor_base(struct nk_context *ctx,
                         vec3 v,
                         float min,
                         float max,
                         float step_btn,
                         float step_drag,
                         const char *label_x,
                         const char *label_y,
                         const char *label_z);

void ui_vec3_editor_rgb(struct nk_context *ctx,
                        vec3 v,
                        float min,
                        float max,
                        float step_btn,
                        float step_drag);


void ui_vec3_editor_xyz(struct nk_context *ctx,
                        vec3 v,
                        float min,
                        float max,
                        float step_btn,
                        float step_drag);


void ui_vec3_editor_deg(struct nk_context *ctx,
                        vec3 v,
                        float min,
                        float max,
                        float step_btn,
                        float step_drag);

#endif /* SAGE_UI_UTIL_H */
