#ifndef SAGE_UI_H
#define SAGE_UI_H

#include "../platform.h"
#include "../scene.h"
#include "ui_scene_graph.h"

/* Forward-declaration */
struct nk_context;

struct ui {
    struct nk_context *context;
    struct ui_scene_graph scene_graph;
};

/* Initializes the Intermediate-Mode GUI using Nuklear */
void ui_init(struct ui *ui, struct platform platform);
void ui_begin_frame(struct ui *ui, struct scene *scene, struct platform *platform);
void ui_end_frame(void);
void ui_shutdown(struct ui *ui);
void ui_process_input(struct ui *ui, struct platform *platform);


#endif /* SAGE_UI_H */
