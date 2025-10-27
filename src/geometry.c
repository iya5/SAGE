/* Geometry source code for Sage

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

#define VERTEX_ATTR_POS 0x01
#define VERTEX_ATTR_UV 0x02
#define VERTEX_ATTR_NORMAL 0x04
#define VERTEX_ATTR_COLOR 0x08

/*
 * We could make a struct that stores vertex positions such as
 * position, uv, normals, colors, but then we lose the flexibility of having
 * a vertex array that might be missing one or several of these attributes. 
 * this decision lead to just using bitmasks flags to choose what attributes
 * are available.
 *
 * this immediately simplifies our work to just storing the vertices as an array
 * without an extra struct
 */
/*
struct vertex_array {
    float *vertices;
    int vertex_count;
};

struct vertex_array vertex_array_load(float *data, size_t n_bytes)
{
    // we'll make the assumption that the total vertex attributes is 5
    // for the pos and uv
    size_t vertex_attr_count = 5;
    
    struct vertex_array *va = malloc(
        (n_bytes / vertex_attr_count) * sizeof(struct vertex)
    );

    if (va == NULL) {
        exit(1);
    }

    int stride = 5;
    int vertex_count = 36;

    struct vertex vertex;

    // add vertices to an array of vertices
    for (int i = 0; i < vertex_count; i++) {

        vertex.position[0] = data[0];
        vertex.position[1] = data[1];
        vertex.position[2] = data[2];

        vertex.uv[0] = data[3];
        vertex.uv[1] = data[4];

        // either we get the normals here or we make them ourselves
        vertex.normal[0] = data[5];
        vertex.normal[1] = data[6];
        vertex.normal[2] = data[7];
    }

    return vertex_array;
}
*/
