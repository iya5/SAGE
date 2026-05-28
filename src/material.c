#include "material.h"
#include "texture.h"
#include "mnf/mnf_util.h"
#include "logger.h"

struct material material_create_default(void)
{
    struct material material = {
        .diffuse_map = texture_create_default(),
        .specular_map = texture_create_default(),
        .shininess = 32
    };

    return material;
}

struct material material_create(const char *diffuse_map_path,
                                const char *specular_map_path,
                                float shininess)
{
    /* TODO: figure out whether i create an array of textures in the scene
       so that i don't end up creating more textures & it makes it easier to
       free memory w/o leaking from forgetting to destroy textures in the GPU */
    if (shininess <= 0) {
        SWARN("Shininess should be greater than 0... clamping it to 1");
        shininess = MIN(shininess, 1);
    }
    struct texture diffuse, specular;

    if (diffuse_map_path == NULL)
        diffuse = texture_create_default();
    else
        diffuse = texture_create(diffuse_map_path);

    if (specular_map_path == NULL)
        specular = texture_create_default();
    else
        specular = texture_create(specular_map_path);

    struct material material = {
        .diffuse_map = diffuse,
        .specular_map = specular,
        .shininess = shininess
    };

    return material;
}

void material_apply(struct shader shader, struct material material)
{
    shader_uniform_1f(shader, "u_material.shininess", material.shininess);
    texture_bind(material.diffuse_map, 0);
    texture_bind(material.specular_map, 1);
}
