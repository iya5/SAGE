/* Camera header: transformation/controls for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __SAGE_CAMERA_H__
#define __SAGE_CAMERA_H__

#include <cglm/cglm.h>

#define CAM_PITCH_MAX 89.0
#define CAM_PITCH_MIN -89.0

#define CAM_DEFAULT_POS (vec3) {0.0, 0.0, 2.0}
#define CAM_DEFAULT_FORWARD (vec3) {0.0, 0.0, -1.0}
#define CAM_DEFAULT_UP  (vec3) {0.0, 1.0, 0.0}

#define PERSPECTIVE_DEFAULT_NEAR 0.1
#define PERSPECTIVE_DEFAULT_FAR 100.0
#define FOV_DEFAULT 67.0
#define FOV_DEFAULT_MIN 4.0
#define FOV_DEFAULT_MAX 98.0

enum movement_type {
    MOVE_FORWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,
    MOVE_BACKWARD,
    MOVE_UP,
    MOVE_DOWN,
};

struct camera {
    vec3 pos;       // the translated position of camera in view space
    vec3 forward;   // the basis vector for z-axis in view space
    vec3 right;     // the basis vector for x-axis in view space (TODO, should  i remove?)
    vec3 world_up;  // the world-y axis view space

    float yaw;      // handles x-axis rotation in degrees
    float pitch;    // handles y-axis rotation in degrees

    float near;     // viewing frustum near plane
    float far;      // viewing frustum far plane
    float aspect;   // viewing frustum width and height 
    float fov;      // viewing frustum angle
    float speed;    // camera movement on coordinate space
    float sensitivity; // camera rotation speed

    mat4 view;
    mat4 projection;
};

void camera_init(struct camera *cam, vec3 pos, vec3 forward, vec3 world_up);
void camera_update(struct camera *cam);

void camera_mouse(struct camera *cam, 
                 float dx, 
                 float dy);

void camera_move(struct camera *cam, int command, double dt);
void camera_scroll(struct camera *cam, float dy);


void camera_perspective(struct camera *cam, 
                        float fov,
                        float aspect,
                        float near,
                        float far);


/* 
 * Constructs a view matrix and writes it to 'view'.
 *
 * Parameters:
 *      view    - Output matrix.
 *      pos     - Position of the camera in world space.
 *      target  - The point the camera is looking at.
 *      up      - World-space vector defining the camera's up direction.
 *
 * The resulting matrix represents the *inverse* of the camera's orientation and
 * position in world space. It transforms all geometry from world space into the
 * camera's local space - known as the *view space* or *eye space*
 * 
 * Thus the camera, after the transformation, becomes the origin and
 * the whole world is reoriented around it. In OpenGL, the camera should be
 * looking down the -z-axis.
 *
 * Conceptually, it converts world space into the camera's local coordinate system
 * (or just a change of basis). This is done by translating the whole world
 * opposite to the camera and rotating it opposite to the camera's orientation.
 * This means we don't physically move the camera but instead move the
 * coordinate system of world space so that the camera is stationary at the
 * origin.
 *      
 * The matrix is used to transform world space to view space.
 */
void view_lookat(mat4 view, vec3 pos, vec3 target, vec3 up);

/* 
 * Constructs a perspective projection matrix and writes it to 'projection'.
 *
 * Parameters:
 *      projection  - Output matrix.
 *      fov         - Field of view in radians; controls how wide the frustum is.
 *      aspect      - Aspect ratio of the viewport (width / height).
 *      near        - Distance to the near clipping plane.
 *      far         - Distance to the far clipping plane.
 *
 * The resulting matrix defines a *viewing frustum* - a truncated pyramid
 * representing the visible region of 3D space.
 *
 * It then compresses the frustum into a 2x2x2 in *clip space*, and after
 * perspective division(dividing each component by its w), into a 2x2x1 cube in
 * normalized device space, commonly known as the unit cube or canonical view
 * volume.
 *
 * This transformation can also be thought of as normalizing the viewing frustum. 
 *
 * This normalization thus warps the vertices in view space giving the effect of
 * perspective: farther objects become smaller and closer to the middle. This
 * matrix prepares the next stage of the GPU for clipping geometry not inside
 * the frustum, thus converting view space into clip space.
 *
 * More can be read here:
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
 */
void projection_perspective(mat4 projection,
                            float fov, 
                            float aspect,
                            float near,
                            float far);

#endif /* __SAGE_CAMERA_H__ */
