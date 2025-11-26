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
