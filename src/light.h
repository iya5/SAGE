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

#ifndef SAGE_LIGHT_H
#define SAGE_LIGHT_H

#include "mnf/mnf_types.h"

enum light_type {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT
};

struct light {
    enum light_type type;
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#endif /* SAGE_LIGHT_H */
