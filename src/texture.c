/* SAGE: Sage Ain't A Game Engine. An OpenGL 3D Renderer

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>

#include "logger.h"
#include "assert.h"
#include "texture.h"

struct texture texture_create(const char *path)
{
	SINFO("Creating texture of %s", path);
	struct texture texture = {0};
	int32_t width, height, channels;

	/* OpenGL expects the uv 0.0 coordinate on the y-axis to be on the bottom
       side of the image */
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	if (data == NULL) {
		SERROR("Texture '%s' failed to load", path);
        return texture;
	}

    /* TODO: */
    GLenum format;
    switch (channels) {
        case 3: format = GL_RGB;
                break;
        case 4: format = GL_RGBA;
                break;
        default: format = GL_RED;
                 break;
    }
    texture.width = width;
    texture.height = height;

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

    /* set texture parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, /* target           */
              0,                /* level (lod)      */
              format,           /* color components */
              width,            /* width            */
              height,           /* height           */
              0,                /* border           */
              GL_RGBA,          /* pixel format     */
              GL_UNSIGNED_BYTE, /* data type        */
              data);            /* data in memory   */

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return texture;
}

uint32_t texture_create_id(const char *path, bool flip)
{
	SINFO("Creating texture id from %s", path);
    uint32_t id;
	int32_t width, height, channels;

	/* OpenGL expects the uv 0.0 coordinate on the y-axis to be on the bottom
       side of the image */
	stbi_set_flip_vertically_on_load(flip);
	unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	if (data == NULL) {
		SERROR("Texture '%s' failed to load", path);
        return 0;
	}

    /* TODO: */
    GLenum format;
    switch (channels) {
        case 3: format = GL_RGB;
                break;
        case 4: format = GL_RGBA;
                break;
        default: format = GL_RED;
                 break;
    }

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

    /* set texture parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, /* target           */
              0,                /* level (lod)      */
              format,           /* color components */
              width,            /* width            */
              height,           /* height           */
              0,                /* border           */
              GL_RGBA,          /* pixel format     */
              GL_UNSIGNED_BYTE, /* data type        */
              data);            /* data in memory   */

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return id;
}


struct texture texture_create_default(void)
{
	SINFO("Creating a default 1x1 pixel texture");
	struct texture texture = {0};

    /* single pixel of (255, 255, 255, 255) */
    const unsigned char white[4] = {255, 255, 255, 255};
    texture.width = 1;
    texture.height = 1;

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

    /* set texture parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, /* target           */
              0,                /* level (lod)      */
              GL_RGBA,          /* color components */
              1,                /* width            */
              1,                /* height           */
              0,                /* border           */
              GL_RGBA,          /* pixel format     */
              GL_UNSIGNED_BYTE, /* data type        */
              white);           /* data in memory   */

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

/*
 * the order for the cubemap is defined in the opengl documentation
 *      +x, -x, +y, -y +z, -z
 * https://wikis.khronos.org/opengl/Cubemap_Texture
 */
struct texture cubemap_texture_create(const char *cubemap_faces[6])
{
    SINFO("Creating a cubemap texture for:");
    SINFO("\t%s", cubemap_faces[0]);
    SINFO("\t%s", cubemap_faces[1]);
    SINFO("\t%s", cubemap_faces[2]);
    SINFO("\t%s", cubemap_faces[3]);
    SINFO("\t%s", cubemap_faces[4]);
    SINFO("\t%s", cubemap_faces[5]);

	struct texture texture = {0};

    /* single pixel of (255, 255, 255, 255) */
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    int32_t width, height, channels;
	width = height = channels = 0;

    /* set texture parameters */
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (size_t i = 0; i < 6; i++) {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(
            cubemap_faces[i], &width, &height, &channels, 0
        );

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,                /* level (lod)      */
                     GL_RGB,           /* color components */
                     width,            /* width            */
                     height,           /* height           */
                     0,                /* border           */
                     GL_RGB,           /* pixel format     */
                     GL_UNSIGNED_BYTE, /* data type        */
                     data);            /* data in memory   */
    }


	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

void cubemap_texture_bind(struct texture t)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, t.id);
}


/* n should be defaulted to 0 if only using 1 uniform texture */
void texture_bind(struct texture t, size_t n_texture_unit)
{
    SASSERT(n_texture_unit < 16);
    glActiveTexture(GL_TEXTURE0 + n_texture_unit);
    glBindTexture(GL_TEXTURE_2D, t.id);
}

void texture_destroy(struct texture *t)
{
    texture_destroy_id(&t->id);
}

void texture_destroy_id(uint32_t *id)
{
    if (*id > 1)
        glDeleteTextures(1, id);
    *id = 0;
}

