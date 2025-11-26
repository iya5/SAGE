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

#ifndef SAGE_UI_SCENE_GRAPH_H
#define SAGE_UI_SCENE_GRAPH_H

#include <stdbool.h>
#include "../darray.h"
#include "../scene.h"

/* Forward Declarations */
struct nk_context;

/* Scene is a flat struct containing a darray of models & lights, a struct to a
   camera, and a struct to a direcctional light (the sun), hence two represent
   the scene in the UI with each of those components as nodes; we either
   restructure the struct to act more as a Scene Graph mimicking scene files
   like GLTF, however, another alternative to this is just to have a UI
   represenation of the scene instead that treats those components as nodes w/o
   needing to change the underlying scene struct itself. The latter was the
   solution that was chosen as it was simpler */
enum ui_node_type {
    UI_NODE_CAMERA,
    UI_NODE_POINT_LIGHT,
    UI_NODE_ENVIRONMENT_LIGHT,
    UI_NODE_MODEL
};

struct ui_scene_node {
    const char *name;
    enum ui_node_type type;
    void *data;
};

struct ui_scene_graph {
    darray *nodes;
    bool dirty;

    struct ui_scene_node *selected_node;
};

void ui_build_scene_graph(struct ui_scene_graph *scene_graph, struct scene *scene);
void ui_draw_scene_graph(struct nk_context *ctx, struct ui_scene_graph *scene_graph);
void ui_rebuild_dirty_scene_graph(struct ui_scene_graph *scene_graph, struct scene *scene);
void ui_draw_inspector(struct nk_context *ctx, struct ui_scene_node *node);

#endif /* SAGE_UI_SCENE_GRAPH_H */
