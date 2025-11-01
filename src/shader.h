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

#ifndef SAGE_SHADER_H
#define SAGE_SHADER_H

#include <stdint.h>
#include "mnf/mnf_types.h"

#define SHADER_PATH_BUFFER_SIZE 1024

/*
 * Represents an OpenGL shader program containing:
 * handle   - id of the program object
 * path     - .glsl file path
 *
 */
struct shader {
    uint32_t handle;
    char path[SHADER_PATH_BUFFER_SIZE];
};

/*
 * Creates a shader program object using OpenGL using a specified glsl shader
 * that combines both vertex and fragment shader relative to the project's root.
 * 
 * It returns a struct containing the handle and file path.
 */
struct shader shader_create(const char *path);

/* Wrapper around glUseProgram */
void shader_use(struct shader *shader);

/* Wrapper around glDeleteProgram */
void shader_destroy(struct shader *shader);

/*
 * Hot reloads a shader program by recompiling the shaders stored in the
 * struct. Assumes the file paths themselves remains unchanged and only the
 * shader sourcce is modified.
 */
void shader_hot_reload(struct shader *shader);

/* for setting uniform states */
void shader_uniform_float(struct shader shader, const char *uniform, float f);
void shader_uniform_vec3(struct shader shader, const char* uniform, vec3 v);
void shader_uniform_vec4(struct shader shader, const char *uniform, vec4 v);
void shader_uniform_mat4(struct shader shader, const char* uniform, mat4 m);

#endif /* SAGE_SHADER_H */
