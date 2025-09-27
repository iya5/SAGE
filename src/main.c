/* Entry point for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <sys/types.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <slog/slog.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void error_callback(int32_t error, const char *description)
{
    fprintf(stderr, "Error (%d): %s\n", error, description);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

char *load_shader_source(const char *path)
{
    FILE *file = NULL;


    file = fopen(path, "r");

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
    int32_t length = ftell(file);
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

int main(int argc, [[maybe_unused]] char **argv)
{
    if (argc > 1) {
        fprintf(stderr, "usage: ./sage\n");
        exit(1);
    }

    printf("hello from sage\n");

    GLFWwindow *window = NULL;
    // slog_log();

    glfwSetErrorCallback(error_callback);

    // GLFW Library initialization
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    /*
    // rendering a triangle
    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.0, 0.5, 0.0
    };

    GLuint vbo;
    // why the hell am i getting a segfault 0x0000000????? with GenBuffers????
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    char *vert_source = load_shader_source("res/shader/shader.vert");
    uint32_t vert_id = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vert_id, 1, (const GLchar * const *) vert_source, NULL);
    glCompileShader(vert_id);
    free(vert_source);
    vert_source = NULL;

    int32_t success;
    char info[512];
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vert_id, 512, NULL, info);
        fprintf(stderr, "Error compiling shader: %s\n", info);
        free(vert_source);
        glfwTerminate();
        exit(1);
    }
    */

    if (window == NULL) {
        glfwTerminate();
        window = NULL;
        return -1;
    }


    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 640, 480);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // swap buffers and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    window = NULL;

    return 0;
}
