/* Math source code for Sage

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include "math.h"
#include <math.h>

/*
 * Linear transformations include rotations, in 2d space, rotations are done
 * about a point, which is the origin (0, 0). The reason why it's only the
 * origin is because linear transformations must maintain the origin's position.
 *
 * 3D rotations also fall under these but instead of rotating about a point, we
 * rotate about an axis (which can be though of as the line of our coordinate
 * spaces).
 *
 * We can rotate using our basis vectors or we can also rotate around an
 * arbitrary axis which is a bit more difficult.
 *
 * The matrices that are formed for transforming the cardinal axis in column
 * major order are:
 *
 * For rotating around the x-axis
 * [1    0     0 ]
 * [0  cosθ -sinθ]
 * [0  sinθ  cosθ]
 *
 * For rotating around the y-axis
 * [cosθ 0 -sinθ]
 * [ 0   1   0  ]
 * [sinθ 0  cosθ]
 *
 * For rotating around the z-axis
 * [cosθ -sinθ 0]
 * [sinθ  cosθ 0]
 * [ 0     0   1]
 *
 * The way these matrices are formed can be taken from first figuring out how
 * roations in 2D space are done
 *
 * ^
 * |         • (tip of triangle) (cosθ, sinθ)
 * |        /|
 * |       / |
 * |      /  |
 * |   1 /   |
 * |    /    | 
 * |   /     | sinθ
 * |  /      |
 * | /       |
 * |/ θ      |
 * +---------+-->
 *     cosθ
 *
 * In a unit circle, the length will always be 1, which is the hypotenus, thus
 * finding the different sides of triangle is simply just the respective
 * trigonometric functions, thus the adjacent is length cosθ and the opposite
 * is just sinθ (relative to angle θ which starts at 0,0 and goes
 * counter-clockwise for positive rotations).
 *
 * But this transformation can be thought of as only being applied to the x-axis
 * or just transforming basis vector î by some angle.
 * We also need to rotate basis vector ĵ (or the y-axis) to maintain symmetry
 * without stretching an object one direction or another, thus giving the
 * illusion of a rotation.
 *
 * Thus ĵ rotated by the same amount(assuming that the î and ĵ are perpendicular
 * to each other, is simply just the same formula plus 90° or π/2.
 *
 * But with the trig identities, it's simplified to:
 * cos(θ + 90°) = -sinθ
 * sin(θ + 90°) = cosθ
 *
 * Thus to rotate an axis by some requires using the matrix transformation of î
 * and ĵ
 * [cosθ -sinθ]
 * [sinθ  cosθ]
 *
 * This rotation is based around a single point(the origin).
 * But it can be applied similar to 3d space when rotation around the cardinal
 * axis. Now connecting the two, rotation around an axis is just the same as
 * applying that formula to the coordinate space if we were to imagine looking
 * down that specific axis we wish to rotate around (our view will end up being
 * a 2d space)
 *
 * Take for example rotating around the x-axis,
 * that is the same as just going to +x and looking straight to the origin. Our
 * view will result to y being our up and down, and z being our left and right.
 *
 * ^ +y
 * |
 * |
 * |
 * |
 * •-----> +z
 *
 * (looking along +x → you)
 *
 * Thus the rotation is only being applied to the y and z columns with the
 * x column using an identity vector of its corresponding axis, In this case,
 * it's just
 *
 *     [1]
* x = [0]
 *     [0]
 *
 * And we only need to transform the y-axis and z-axis using the same formula
 * taken from the 2D coordinate rotation but only applied to those axis, thus
 * leaving the x component for that column equal to 0.
 * 
 * Resulting in the above formula of:
 * [1    0     0 ]
 * [0  cosθ -sinθ]
 * [0  sinθ  cosθ]
 *
 *
 * Since we are handling model matrices as the principal reason for rotation,
 * we will use a 4x4 identity matrix and ignore the w component and the 4th
 * column
 */

void rotate_x(mat4 in, float theta, mat4 out)
{
    /*
     * For rotating around the x-axis
     * [1    0     0 ]
     * [0  cosθ -sinθ]
     * [0  sinθ  cosθ]
     */
    mat4 r = {
        {1,          0,           0, 0},
        {0, cos(theta), -sin(theta), 0},
        {0, sin(theta),  cos(theta), 0},
        {0,          0,           0, 1},
    };

    glm_mat4_mul

    glm_mat4_copy(temp, out);
}

void rotate_y(mat4 in, float theta, mat4 out)
{
    /* For rotating around the y-axis
     * [cosθ 0 -sinθ]
     * [ 0   1   0  ]
     * [sinθ 0  cosθ]
     */
    mat4 temp = {
        {in[0][0] * cos(theta), in[0][1], in[0][2] * -sin(theta), in[0][3]},
        {in[1][0], in[1][1], in[1][2], in[1][3]},
        {in[2][0] * sin(theta), in[2][1], in[2][2] * cos(theta), in[2][3]},
        {in[3][0], in[3][1], in[3][2], in[3][3]},
    };

    glm_mat4_copy(temp, out);
}

void rotate_z(mat4 in, float theta, mat4 out)
{
    /*
     * For rotating around the z-axis
     * [cosθ -sinθ 0]
     * [sinθ  cosθ 0]
     * [ 0     0   1]
     */
    mat4 temp = {
        {in[0][0] * cos(theta), in[0][1] * -sin(theta), in[0][2], in[0][3]},
        {in[1][0] * sin(theta), in[1][1] * cos(theta), in[1][2], in[1][3]},
        {in[2][0], in[2][1], in[2][2], in[2][3]},
        {in[3][0], in[3][1], in[3][2], in[3][3]},
    };

    glm_mat4_copy(temp, out);
}

/* wrapper for vec3 rotating all three euler angles x->y->z */
void rotate_euler_xyz(mat4 mat, vec3 euler_angles)
{
    rotate_x(mat, euler_angles[X], mat);
    rotate_y(mat, euler_angles[Y], mat);
    rotate_z(mat, euler_angles[Z], mat);
}
