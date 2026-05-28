#ifndef SAGE_OBJ_LOADER_H
#define SAGE_OBJ_LOADER_H

#include "mesh.h"

/* Loads a mesh from an .obj file and pushes the data onto the parameters set.
   But if vertices or indices are declared but have not been allocated, the
   function allocates memory to it, however they MUST be defined as NULL */
void obj_load_mesh(const char *path, darray **vertices, darray **indices);

#endif /* SAGE_OBJ_LOADER_H */
