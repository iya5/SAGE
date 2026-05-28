#include "camera.h"
#include <math.h>
#include "mnf/mnf_matrix.h"
#include "mnf/mnf_vector.h"
#include "mnf/mnf_util.h"

void camera_init(struct camera *cam, vec3 pos, vec3 forward, vec3 world_up)
{
    mnf_vec3_copy(pos, cam->pos);
    mnf_vec3_copy(forward, cam->forward);
    mnf_vec3_copy(world_up, cam->world_up);
    mnf_vec3_normalize(cam->world_up, cam->world_up);
    mnf_vec3_normalize(cam->forward, cam->forward);

    cam->speed = CAM_DEFAULT_SPEED;
    cam->sensitivity = CAM_DEFAULT_SENSITIVITY;
    // reason for -90 is because we're offset facing the left
    cam->yaw = -90.0 - 45;
    cam->pitch = 0 - 25;

    /* recomputing the forward based on the yaw and pitch defaults */
    mnf_vec3_copy(
        (vec3) {
            cos(MNF_RAD(cam->yaw)) * cos(MNF_RAD(cam->pitch)),
            sin(MNF_RAD(cam->pitch)),
            sin(MNF_RAD(cam->yaw)) * cos(MNF_RAD(cam->pitch))
        },
        cam->forward
    );
    mnf_vec3_normalize(cam->forward, cam->forward);


    cam->can_move = false;

    mnf_mat4_identity(cam->view);
    mnf_mat4_identity(cam->projection);
    vec3 target;
    mnf_vec3_add(cam->pos, cam->forward, target);
    view_lookat(cam->view, cam->pos, target, cam->world_up);
}

void camera_perspective(struct camera *cam, 
                        float fov,
                        float aspect,
                        float near,
                        float far)
{
    cam->near = near;
    cam->far = far;
    cam->aspect = aspect;
    cam->fov = fov;
    projection_perspective(cam->projection, MNF_RAD(fov), aspect, near, far);
}

void camera_move(struct camera *cam, int command, double dt)
{
    if (command == MOVE_FORWARD) {
        vec3 delta;
        mnf_vec3_scale(cam->forward, cam->speed * dt, delta);
        mnf_vec3_add(cam->pos, delta, cam->pos);
    }

    if (command == MOVE_BACKWARD) {
        vec3 delta;
        mnf_vec3_scale(cam->forward, -cam->speed * dt, delta);
        mnf_vec3_add(cam->pos, delta, cam->pos);
    }

    if (command == STRAFE_LEFT) {
        vec3 right;
        mnf_vec3_cross(cam->forward, cam->world_up, right);
        mnf_vec3_normalize(right, right);
        mnf_vec3_scale(right, -cam->speed * dt, right);
        mnf_vec3_add(cam->pos, right, cam->pos);
    }

    if (command == STRAFE_RIGHT) {
        vec3 right;
        mnf_vec3_cross(cam->forward, cam->world_up, right);
        mnf_vec3_normalize(right, right);
        mnf_vec3_scale(right, cam->speed * dt, right);
        mnf_vec3_add(cam->pos, right, cam->pos);
    }

    if (command == MOVE_UP)
        cam->pos[Y] += cam->speed * dt; 

    if (command == MOVE_DOWN)
        cam->pos[Y] -= cam->speed * dt;
}

void camera_mouse(struct camera *cam, float dx, float dy)
{
    if (!cam->can_move) return;
    cam->yaw += dx * cam->sensitivity;
    cam->pitch += dy * cam->sensitivity;

    cam->pitch = CLAMP(cam->pitch, CAM_PITCH_MIN, CAM_PITCH_MAX);

    // explanation of how in https://learnopengl.com/Getting-started/Camera
    mnf_vec3_copy(
        (vec3) {
            cos(MNF_RAD(cam->yaw)) * cos(MNF_RAD(cam->pitch)),
            sin(MNF_RAD(cam->pitch)),
            sin(MNF_RAD(cam->yaw)) * cos(MNF_RAD(cam->pitch))
        },
        cam->forward
    );
    mnf_vec3_normalize(cam->forward, cam->forward);

}

void camera_scroll(struct camera *cam, float dy)
{
    cam->fov -= dy;
    cam->fov = CLAMP(cam->fov, FOV_DEFAULT_MIN, FOV_DEFAULT_MAX);
}

void camera_update(struct camera *cam)
{
    vec3 target;
    mnf_vec3_add(cam->pos, cam->forward, target);
    view_lookat(cam->view, cam->pos, target, cam->world_up);
    projection_perspective(cam->projection, 
                           MNF_RAD(cam->fov), 
                           cam->aspect, cam->near, 
                           cam->far);
}

void view_lookat(mat4 view, vec3 pos, vec3 target, vec3 up)
{
    vec3 distance = {
        target[X] - pos[X],
        target[Y] - pos[Y],
        target[Z] - pos[Z]
    };

    vec3 cam_forward;
    mnf_vec3_normalize(distance, cam_forward);

    // normalizing prevents some really weird distortions. Why? because they are
    // our basis vectors for the coordinate system and must be unit vectors
    vec3 cam_right, cam_up;
    mnf_vec3_cross(cam_forward, up, cam_right);
    mnf_vec3_normalize(cam_right, cam_right);

    // recalculating the up vector for consistency
    mnf_vec3_cross(cam_right, cam_forward, cam_up);
    mnf_vec3_normalize(cam_up, cam_up);

    vec3 cam_translate = {
        mnf_vec3_dot(cam_right, pos),
        mnf_vec3_dot(cam_up, pos),
        mnf_vec3_dot(cam_forward, pos),

    };

    mat4 temp = {
        {cam_right[X], cam_up[X], -cam_forward[X], 0},
        {cam_right[Y], cam_up[Y], -cam_forward[Y], 0},
        {cam_right[Z], cam_up[Z], -cam_forward[Z], 0},
        {-cam_translate[X], -cam_translate[Y], cam_translate[Z], 1}
    };

    mnf_mat4_copy(temp, view);
}


void projection_perspective(mat4 projection,
                            float fov, 
                            float aspect,
                            float near,
                            float far)
{
    float range = tan(fov * 0.5) * near;

    float Sx = (2.0 * near) / (range * aspect + range * aspect);
    float Sy = near / range;
    float Sz = -(far + near) / (far - near);

    float Pz = -(2 * far * near) / (far - near);

    mat4 temp = {
        {Sx, 0, 0, 0},
        {0, Sy, 0, 0},
        {0, 0, Sz, -1},
        {0, 0, Pz, 0}
    };
    mnf_mat4_copy(temp, projection);
}
