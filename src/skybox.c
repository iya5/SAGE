#include <glad/gl.h>
#include "skybox.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "mnf/mnf_matrix.h"
#include "logger.h"

struct shader skybox_shader;

void skybox_init(struct skybox *skybox, const char *cubemap_paths[6])
{
    SINFO("Initializing skybox");
    if (skybox_shader.handle == 0)
        skybox_shader = shader_create("glsl/skybox.glsl");

    skybox->mesh = mesh_geometry_create_cube();
    skybox->cubemap = cubemap_texture_create(cubemap_paths);
}

void skybox_draw(struct skybox skybox, mat4 view, mat4 projection)
{
    shader_use(skybox_shader);
    mesh_bind(skybox.mesh);
    cubemap_texture_bind(skybox.cubemap);


    mat4 view_no_translation;
    mnf_mat4_copy(view, view_no_translation);

    view_no_translation[3][0] = 0;
    view_no_translation[3][1] = 0;
    view_no_translation[3][2] = 0;
    view_no_translation[3][3] = 1;

    shader_uniform_mat4(skybox_shader, "u_view", view_no_translation);
    shader_uniform_mat4(skybox_shader, "u_projection", projection);
    glDepthMask(GL_FALSE);
    mesh_draw(skybox.mesh);
    glDepthMask(GL_TRUE);
}

void skybox_destroy(struct skybox *skybox)
{
    SINFO("Destroying skybox");
    texture_destroy(&skybox->cubemap);
    mesh_destroy(&skybox->mesh);
}
