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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <glad/gl.h>

#include "shader.h"
#include "logger.h"

#define DEFINE_VERSION      "#version 410 core\n"
#define DEFINE_VS           "#define COMPILE_VS\n"
#define DEFINE_FS           "#define COMPILE_FS\n"
#define INJECT_DEFINE_VS    DEFINE_VERSION DEFINE_VS
#define INJECT_DEFINE_FS    DEFINE_VERSION DEFINE_FS
/* defines the number of strings passed to glShaderSource */
#define SHADER_SRC_N_STR 2

static char *shader_load_from_source(const char *path);

struct shader shader_create(const char *path)
{
    struct shader shader = {0};
    int32_t success = 0;
    char info[512] = {0};

    SINFO("Compiling shader '%s'", path);

    char *source = shader_load_from_source(path);
    if (source == NULL) {
        SFATAL("Failed to load shader source '%s' to program.", path);
        SFATAL("Ensure the path to the shader exists.");
        exit(1);
    }

    /* compiling vertex shader */
    uint32_t vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 
                   SHADER_SRC_N_STR,
                   (const GLchar * const []) {INJECT_DEFINE_VS, source},
                   NULL);
    glCompileShader(vs_id);
    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs_id, 512, NULL, info);
        SERROR("Vertex shader '%s' compilation error:", path);
        SERROR("%s", info);
    }

    /* compiling fragment shader */
    uint32_t fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id,
                   SHADER_SRC_N_STR,
                   (const GLchar * const []) {INJECT_DEFINE_FS, source},
                   NULL);
    glCompileShader(fs_id);
    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fs_id, 512, NULL, info);
        SERROR("Fragment shader '%s' compilation error:", path);
        SERROR("%s", info);
    }

    free(source);

    /* creating shader program */
    uint32_t id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, info);
        SERROR("Shader '%s' linking failure:", path);
        SERROR("%s", info);
        id = 0;
    }

    glDeleteShader(fs_id);
    glDeleteShader(vs_id);

    shader.handle = id;

    int i = 0;
    for (i = 0; i < SHADER_PATH_BUFFER_SIZE  - 1 && path[i] != '\0'; i++)
        shader.path[i] = path[i];
    shader.path[i] = '\0';

    return shader;
}

void shader_hot_reload(struct shader *shader)
{
    struct shader reloaded_shader = shader_create(shader->path);
    if (!reloaded_shader.handle) {
        SWARN("Shader '%s' hot-reload failed", shader->path);
    } else {
        SDEBUG("Hot-reloading shader '%s'", shader->path);
        glDeleteProgram(shader->handle);
        shader->handle = reloaded_shader.handle;
    }
}

void shader_use(struct shader shader)
{
    glUseProgram(shader.handle);
}

void shader_destroy(struct shader *shader)
{
    glDeleteProgram(shader->handle);
    shader->handle = 0;
}

void shader_uniform_vec4(struct shader shader, const char *uniform, vec4 v)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        SDEBUG("Uniform '%s' doesn't exist in '%s'", uniform, shader.path);
    } else {
        glUniform4f(location, v[0], v[1], v[2], v[3]);
    }
}

void shader_uniform_mat4(struct shader shader, const char *uniform, mat4 m)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        SDEBUG("Uniform '%s' doesn't exist in '%s'", uniform, shader.path);
    } else {
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)m);
    }

}

void shader_uniform_1i(struct shader shader, const char *uniform, int32_t n)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        SDEBUG("Uniform '%s' doesn't exist in '%s'", uniform, shader.path);
    } else {
        glUniform1i(location, n);
    }
}

void shader_uniform_1f(struct shader shader, const char *uniform, float f)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        SDEBUG("Uniform '%s' doesn't exist in '%s'", uniform, shader.path);
    } else {
        glUniform1f(location, f);
    }
}

void shader_uniform_vec3(struct shader shader, const char *uniform, vec3 v)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        SDEBUG("Uniform '%s' doesn't exist in '%s'", uniform, shader.path);
    } else {
        glUniform3f(location, v[0], v[1], v[2]);
    }

}

static char *shader_load_from_source(const char *path)
{
    FILE *file = NULL;
    char *source = NULL;
    long length = 0;
    size_t bytes_read = 0;

    file = fopen(path, "rb");
    if (file == NULL) goto err;

    if (fseek(file, 0, SEEK_END) == -1) goto err;
    length = ftell(file);
    if (length == -1) goto err;
    if (fseek(file, 0, SEEK_SET) == -1) goto err;

    source = (char *) malloc(length + 1);
    if (source == NULL) goto err;

    bytes_read = fread(source, sizeof(char), length, file);
    if (bytes_read != (size_t) length) goto err;

    source[length] = '\0';
    fclose(file);
    return source;

err:
    SERROR("Failed to load shader '%s' onto memory", path);
    if (file) fclose(file);
    free(source);
    return NULL;
}

