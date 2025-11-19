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

#define _POSIX_C_SOURCE 200809L
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "obj_loader.h"
#include "logger.h"
#include "mnf/mnf_vector.h"
#include "mesh.h"

#define LINE_INITIAL_SIZE 512

darray *obj_load_from_file(const char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        SFATAL("Failed to open .obj file '%s'", path);
        exit(1);
    }

    size_t size = LINE_INITIAL_SIZE;
    char *line = malloc(size);
    if (line == NULL) {
        SFATAL("Failed to alloc memory for reading .obj file '%s'", path);
        fclose(file);
        exit(1);
    }

    darray *positions = darray_alloc(sizeof(vec3), 1028);
    darray *normals = darray_alloc(sizeof(vec3), 1028);
    darray *uvs = darray_alloc(sizeof(vec2), 1028);
    darray *vertices = darray_alloc(sizeof(struct vertex), 1028);

    ssize_t nread;
    while ((nread = getline(&line, &size, file)) != -1) {

        if ('#' == line[0]) continue;

        /* position */
        if (strncmp("v ", line, 2) == 0) {
            vec3 position;
            sscanf(line,
                   "v %f %f %f",
                   &position[0],
                   &position[1],
                   &position[2]);
            darray_push(positions, position);
        }

        /* normal */
        if (strncmp("vn ", line, 3) == 0) {
            vec3 normal;
            sscanf(line,
                   "vn %f %f %f",
                   &normal[0],
                   &normal[1],
                   &normal[2]);
            mnf_vec3_normalize(normal, normal);
            darray_push(normals, normal);
        }

        /* uv */
        if (strncmp("vt ", line, 3) == 0) {
            vec2 uv;
            sscanf(line,
                   "vt %f %f",
                   &uv[0],
                   &uv[1]);
            darray_push(uvs, uv);
        }

        if (strncmp("f ", line, 2) == 0) {
            int32_t v_i[3];
            int32_t t_i[3];
            int32_t n_i[3];
            sscanf(line,
                   "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &v_i[0], &t_i[0], &n_i[0],
                   &v_i[1], &t_i[1], &n_i[1],
                   &v_i[2], &t_i[2], &n_i[2]);

            for (size_t i = 0; i < 3; i++) {
                /* offset by one because obj files start indexing at 1 */
                vec3 *position = darray_at(positions, v_i[i] - 1);
                vec3 *normal = darray_at(normals, n_i[i] - 1);
                vec2 *uv = darray_at(uvs, t_i[i] - 1);

                struct vertex vertex;
                mnf_vec3_copy(*position, vertex.pos);
                mnf_vec3_copy(*normal, vertex.normal);
                mnf_vec2_copy(*uv, vertex.uv);

                darray_push(vertices, &vertex);
            }
        }
    }

    free(line);
    darray_free(positions);
    darray_free(uvs);
    darray_free(normals);
    fclose(file);
    
    return vertices;
}

