/*
This file is part of SAGE.
SAGE is free software: you can redistribute it and/or modify it under the 
terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later 
version.

SAGE is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR 
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
SAGE. If not, see <https://www.gnu.org/licenses/>. 
 */

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error (%d): %s\n", error, description);
}

int main(void)
{
    printf("hello from sage\n");

    GLFWwindow *window = NULL;

    glfwSetErrorCallback(error_callback);

    // GLFW Library initialization
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        window = NULL;
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    window = NULL;

    return 0;
}
