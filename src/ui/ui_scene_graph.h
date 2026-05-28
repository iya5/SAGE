#ifndef SAGE_UI_SCENE_GRAPH_H
#define SAGE_UI_SCENE_GRAPH_H

#include <stdbool.h>
#include "../darray.h"
#include "../scene.h"

/* Forward Declarations */
struct nk_context;

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
