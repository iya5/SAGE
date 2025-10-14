/* Shader header for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __SAGE_SHADER_H__
#define __SAGE_SHADER_H__

#include <stdint.h>
#include <cglm/cglm.h>

#define SHADER_PATH_BUFFER_SIZE 1024

/*
 * Represents an OpenGL shader program containing:
 *      handle  -> id of the program object
 *      vs_path -> relative path to the vertex shader
 *      fs_path -> relative path to the fragment shader
 */
struct shader_program {
    uint32_t handle;
    char vs_path[SHADER_PATH_BUFFER_SIZE];
    char fs_path[SHADER_PATH_BUFFER_SIZE];
};

/*
 * Creates a shader program object using OpenGL using a specified vertex and
 * fragment shader path relative to the project's root directory. Returns a
 * struct containing the OpenGL handle and shader file paths.
 */
struct shader_program shader_program_create(const char *vs_path, 
                                            const char *fs_path);

/* Wrapper around glUseProgram */
void shader_program_use(struct shader_program *program);

/* Wrapper around glDeleteProgram */
void shader_program_destroy(struct shader_program *program);

/*
 * Hot reloads a shader program by recompiling the shaders stored in the
 * struct. Assumes the file paths themselves remains unchanged and only the
 * shader sourcce is modified.
 */
void shader_program_hot_reload(struct shader_program *program);

/**/

void shader_program_uniform_vec3(struct shader_program program,
                                 const char* uniform,
                                 vec3 v);

void shader_program_uniform_vec4(struct shader_program program, 
                                 const char *uniform, 
                                 vec4 v);

void shader_program_uniform_mat4(struct shader_program program, 
                                 const char* uniform,
                                 mat4 m);

#endif /* __SAGE_SHADER_H__ */
