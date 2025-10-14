 /*
     * A problem with rotation matrices occur however, when simply doing these
     * in a hierachy of rotation axises
     * Namely, once a vector is parallel to another unit vector (representing
     * the rotation axis), its rank ends up decreasing, thus locking its
     * transformation to that unit vector, making it linearly dependent.
     * This is what's known as a Gimbal Lock. This ends with us losing an axis
     * of rotation because we can no longer reach the full span of 3D space.
     *
     * In layman's: when two rotation handles line up, turning one is the
     * same as turning the other, loosing axis of rotation of the other.
     *
     * Solutions such as clamping before it's parallel to a unit vector can be
     * made, but it becomes very messy to deal with, thus rotations are typically
     * done using quaternions
     *
     * Below are the rotation axis formulas (in column major) as a proof of
     * conecpt
     *
     * Rotating around the x:
     * [1,    0,     0, 0] [x]     [      1      ]
     * [0, cosθ, -sinθ, 0] [y]  =  [cosθy - sinθz]
     * [0, sinθ,  cosθ, 0] [z]     [sinθy + cosθz]
     * [0,    0,     0, 1] [w]     [      1      ]
     *
     * Rotating around the y:
     * [ cosθ,    0, sinθ, 0] [x]     [ cosθx - sinθz]
     * [    0,    1,    0, 0] [y]  =  [       y      ]
     * [-sinθ,    0, cosθ, 0] [z]     [-sinθx + cosθz]
     * [    0,    0,    0, 1] [w]     [       1      ]
     *
     * Rotating around the x:
     * [cosθ, -sinθ, 0, 0] [x]     [cosθx - sinθy]
     * [sinθ,  cosθ, 0, 0] [y]  =  [sinθx + cosθy]
     * [   0,     0, 1, 0] [z]     [      z      ]
     * [   0,     0, 0, 1] [w]     [      1      ]
     *
     * A better matrice can be done to avoid gimbal lock by rotating around an
     * arbitrary unit axis but still does not solve the issue. This matrix is
     * referenced in https://learnopengl.com/Getting-started/Transformations
     */


