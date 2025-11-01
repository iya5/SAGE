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

#ifndef SAGE_CONFIG_H
#define SAGE_CONFIG_H

#define SAGE_WINDOW_TITLE   "Sage"

#define SAGE_MULTISAMPLE_ANTIALIASING 8

// constants are based off glfwSwapInterval()
#define SAGE_VSYNC_LOCK     1
#define SAGE_VSYNC_UNLOCK   0
#define SAGE_VSYNC_SETTING  SAGE_VSYNC_LOCK

#define SAGE_OPENGL_MAJOR_VERSION 4
#define SAGE_OPENGL_MINOR_VERSION 1

#endif /* SAGE_CONFIG_H */
