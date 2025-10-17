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

static char *shader_load_from_source(const char *path);

struct shader shader_create(const char *vs_path, const char *fs_path)
{
    struct shader shader = {0};

    int32_t success = 0;
    char info[512] = {0};

    // compiling the vertex shader
    LOG_INFO("Compiling '%s'", vs_path);
    char *vs_source = shader_load_from_source(vs_path);
    uint32_t vs_id = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vs_id, 1, (const GLchar * const *) &vs_source, NULL);
    glCompileShader(vs_id);
    free(vs_source);
    vs_source = NULL;

    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs_id, 512, NULL, info);
        LOG_ERROR("Failed to compile '%s': %s", vs_path, info);
    }

    // compiling the fragment shader
    LOG_INFO("Compiling '%s'", fs_path);
    char *fs_source = shader_load_from_source(fs_path);
    uint32_t fs_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fs_id, 1, (const GLchar * const *) &fs_source, NULL);
    glCompileShader(fs_id);
    free(fs_source);
    fs_source = NULL;

    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fs_id, 512, NULL, info);
        LOG_ERROR("Failed to compile shader: %s", info);
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

    size_t i = 0;
    for (i = 0; i < SHADER_PATH_BUFFER_SIZE - 1 && vs_path[i] != '\0'; i++)
        shader.vs_path[i] = vs_path[i];
    shader.vs_path[i] = '\0';

    i = 0;
    for (i = 0; i < SHADER_PATH_BUFFER_SIZE  - 1 && fs_path[i] != '\0'; i++)
        shader.fs_path[i] = fs_path[i];
    shader.fs_path[i] = '\0';

    return shader;
}

void shader_hot_reload(struct shader *shader)
{

    struct shader reloaded_shader = shader_create(shader->vs_path, 
                                                  shader->fs_path);
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
        LOG_WARN("Accessing uniform '%s' doesn't exist", uniform);
    } else {
        glUniform4f(location, v[0], v[1], v[2], v[3]);
    }
}

void shader_uniform_mat4(struct shader shader, const char* uniform, mat4 m)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        LOG_WARN("Accessing uniform '%s' doesn't exist", uniform);
    } else {
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)m);
    }

}

void shader_uniform_vec3(struct shader shader, const char* uniform, vec3 v)
{
    int32_t location = glGetUniformLocation(shader.handle, uniform);
    if (location < 0) {
        LOG_WARN("Accessing uniform '%s' doesn't exist", uniform);
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
