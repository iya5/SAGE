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

#ifndef SAGE_MESH_H
#define SAGE_MESH_H

#include <stdint.h>
#include <stddef.h>

#include "mnf/mnf_types.h"
#include "darray.h"

/* The mesh defines the actual geometry of the thing we are rendering, such as
   shapes or a flat plane, or even complex meshes. One thing about meshes is
   that they should ideally not store any information regarding how they
   actually look besides the geometry of the mesh. This means a mesh is just a
   container of vertices (and indices for optimizations!). It is simply geometry
   and contains no references to things such as textures or materials.

   Now a vertex is just a point in space, whether in 2D or 3D, with attributes.
   When thinking of a vertex, we can just think of a point's position. The
   additional attributes however are used for information such as texture
   mapping and lighting inside Sage: respectively they are the uv and normal.

   These attributes are vectors in 3D space and should not generally be thought
   of as points but rather arrows with a magnitude and direction, but for now,
   points can and probably could suffice */
struct vertex {
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

/* After defining what a vertex is and what it contains, to send that
   information over to the GPU, we cannot just directly pump it through a data
   bus. There are prerequisite stages to it before it is done. These are
   required by OpenGL and are called the Vertex Array Objects. They define the
   layout positions of the vertex arrays sent to the GPU, so for example, our
   vertices can be laid out like such:
        x y z | r g b | x y z | r g b | ... and so on.
    Or we can also interleave them as some renders do:
        x r y g b z | x r y g b z | ... and so on.

    If you noticed, these Vertex Array Objects or for short, VAOs, do not
    actually store vertices directly, only the layout of the vertices in the GPU
    but we use Vertex Buffer Objects for the actual data we want to send over.

    Vertex Buffer Objects or VBOs are the data we attach our vertices to and is
    attached to the VAO so we can interpret the meaning of the VBO. VBOs are
    then the container and needs to be defined on the GPU for drawing our data!

    Simply, the VAO acts as the dictionary that gives meaning to the VBO, and
    the VBO is the actual container for the data. The two are important!

    After defining those two, we can also additional do some optimizations such
    as using IBOs but those won't be defined for now as they are not as
    important in terms of learning principles. But feel free to read more as
    they are the standard for drawing meshes and saves a ton of space. */
struct mesh_gpu {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;
    uint32_t vertex_count;
    uint32_t index_count;
};

/* Finally, we define the mesh as containing principally the memory buffer
   information on the GPU as well as the vertices (and the indices for
   optimizations but are not a hard requirement).

   This struct will be used for drawing, creating, destroying, and binding our
   mesh!

   We say binding a mesh because before we can draw a mesh, the GPU needs to
   know what mesh it will draw. The process therefore is the CPU sends over the
   VAOs and VBOs to the GPU as well as the vertices to the GPU's VRAM. The CPU
   then binds whichever mesh it needs to GPU to render before sending a draw
   call to the GPU. This is how the GPU knows what vertices it needs to draw.

   (DO NOTE THAT THE ABOVE IS BASED ON OPENGL'S APPLICATION AS OPENGL IS A STATE
   MACHINE AND SHOULD THOUGHT OF AS SUCH) */
struct mesh {
    struct mesh_gpu buffer;
    darray *vertices;
    darray *indices;
};

/* Now we go over to our basic functions for drawing a mesh, which are creating
   a basic cube (so this function simply hardcodes the vertices), then we can
   create a mesh by providing vertices and or indices to it. This part is where
   we use a parser, in our case the obj loader, to retrieve the vertices which
   are then sent over to function.

   Then we destroy a mesh by freeing up its data and memory location in the GPU
   at the end of its lifetime, then there are the binding and drawing which
   should always be used in tandem: Binding the mesh first before drawing it! */
struct mesh mesh_geometry_create_cube(void);
struct mesh mesh_create(darray *vertices, darray *indices);
void mesh_destroy(struct mesh *mesh);
void mesh_bind(struct mesh mesh);
void mesh_draw(struct mesh mesh);

#endif /* SAGE_MESH_H */
