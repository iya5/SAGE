/* Texture source code for compiling and loading textures for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>
#include <slog/slog.h>

#include <string.h>

#include "texture.h"

struct texture texture_create(const char *path)
{
	LOG_INFO("Creating texture of %s", path);
	struct texture texture = {0};
	int32_t width, height, channels;
	width = height = channels = 0;

	// OpenGL expects the uv 0.0 coordinate on the y-axis
	// to be on the bottom side of the image
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(
		path, &width, &height, &channels, STBI_rgb_alpha
	);

	if (data == NULL) {
		LOG_ERROR("Failed to load texture '%s'", path);
        return texture;
	}

	memcpy(&texture.size, (ivec2) {width, height}, sizeof(ivec2));

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// generting texture
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		data
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	// clean up and unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return texture;
}

void texture_bind(struct texture t)
{
    glBindTexture(GL_TEXTURE_2D, t.id);
}

void texture_destroy(struct texture *t)
{
    if (t->id > 1) {
        glDeleteTextures(1, &t->id);
        t->id = 0;
    }
}

