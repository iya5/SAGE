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

#ifndef SAGE_OBJ_LOADER_H
#define SAGE_OBJ_LOADER_H

#include "mesh.h"

/* Loads a mesh from an .obj file and pushes the data onto the parameters set.
   But if vertices or indices are declared but have not been allocated, the
   function allocates memory to it, however they MUST be defined as NULL */
void obj_load_mesh(const char *path, darray **vertices, darray **indices);

#endif /* SAGE_OBJ_LOADER_H */
