/* Shader source for compiling and creating shader programs for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <glad/gl.h>
#include <slog/slog.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "shader.h"

#define DEF_VERSION "#version 410 core\n"
#define DEF_VS      "#define COMPILE_VS\n"
#define DEF_FS      "#define COMPILE_FS\n"

static char *shader_load_from_source(const char *path);

struct shader shader_create(const char *path)
{
    struct shader shader = {0};

    int32_t success = 0;
    char info[512] = {0};

    // compiling the vertex shader
    LOG_INFO("Compiling vertex shader '%s'", path);
    char *vs_source[] = {
        DEF_VERSION,
        DEF_VS,
        shader_load_from_source(path)
    };
    uint32_t vs_id = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vs_id, 3, (const GLchar * const *) &vs_source, NULL);
    glCompileShader(vs_id);
    free(vs_source[2]);
    vs_source[2] = NULL;

    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs_id, 512, NULL, info);
        LOG_ERROR("Failed to compile vertex shader '%s': %s", path, info);
    }

    // compiling the fragment shader
    LOG_INFO("Compiling fragment shader '%s'", path);
    char *fs_source[] = {
        DEF_VERSION,
        DEF_FS,
        shader_load_from_source(path)
    };
    uint32_t fs_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fs_id, 3, (const GLchar * const *) &fs_source, NULL);
    glCompileShader(fs_id);
    free(fs_source[2]);
    fs_source[2] = NULL;

    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fs_id, 512, NULL, info);
        LOG_ERROR("Failed to compile fragment shader '%s': %s", path, info);
    }

    // creating shader program
    uint32_t id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, info);
        LOG_ERROR("Failed to link shader program: %s", info);
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
        LOG_WARN("Failed to hot reload");
        glDeleteProgram(reloaded_shader.handle);
    } else {
        LOG_INFO("Hot reloading shader");
        glDeleteProgram(shader->handle);
        shader->handle = reloaded_shader.handle;
    }
}

void shader_use(struct shader *shader)
{
    glUseProgram(shader->handle);
}

void shader_destroy(struct shader *shader)
{
    glDeleteProgram(shader->handle);
}

void shader_uniform_vec4(struct shader shader, const char *uniform, vec4 v)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        LOG_WARN("Uniform '%s' doesn't exist in %s", uniform, shader.path);
    } else {
        glUniform4f(location, v[0], v[1], v[2], v[3]);
    }
}

void shader_uniform_mat4(struct shader shader, const char* uniform, mat4 m)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        LOG_WARN("Uniform '%s' doesn't exist in %s", uniform, shader.path);
    } else {
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)m);
    }

}

void shader_uniform_vec3(struct shader shader, const char* uniform, vec3 v)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        LOG_WARN("Uniform '%s' doesn't exist in %s", uniform, shader.path);
    } else {
        glUniform3f(location, v[0], v[1], v[2]);
    }

}

static char *shader_load_from_source(const char *path)
{
    FILE *file = NULL;

    file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open shader: %s\n", path);
        exit(1);
    }

    // grabbing the total bytes of the shader
    if (fseek(file, 0, SEEK_END) == -1) {
        fprintf(stderr, "Failure during fseek\n");
        fclose(file);
        exit(1);
    }
    int64_t length = ftell(file);
    if (length == -1) {
        fprintf(stderr, "Failure during ftell\n");
        fclose(file);
        exit(1);
    }
    if (fseek(file, 0, SEEK_SET) == -1) {
        fprintf(stderr, "Failure during fseek\n");
        fclose(file);
        exit(1);
    }

    char *source = (char *) malloc(length + 1);
    if (source == NULL) {
        fprintf(stderr, "Failed to allocate memory during shader load\n");
        fclose(file);
        exit(1);
    }
    size_t bytes_read = fread(source, sizeof(char), length, file);

    if (ferror(file) != 0) {
        fprintf(stderr, "Failed to fread shader\n");
        fclose(file);
        free(source);
        exit(1);
    }

    if ((int32_t) bytes_read != length) {
        fprintf(stderr, "could not read total bytes from the shader\n");
        fclose(file);
        free(source);
        exit(1);
    }
    
    source[length] = '\0';

    fclose(file);
    return source;
}
