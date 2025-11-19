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

#ifndef SAGE_UI_H
#define SAGE_UI_H

#include "platform.h"
#include "scene.h"

/* Forward-declaration */
struct nk_context;

struct ui {
    struct nk_context *context;
    bool hovered;
};

/* Initializes the Intermediate-Mode GUI using Nuklear */
void ui_init(struct ui *ui, struct platform platform);
void ui_draw(struct ui *ui, struct scene *scene, struct platform *platform);
void ui_process_input(struct ui *ui, struct platform *platform);
void ui_render(void);

/* Shutdowns the IMGUI */
void ui_shutdown(struct ui *ui);

#endif /* SAGE_UI_H */
