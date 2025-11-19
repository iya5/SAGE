/*
void ray_cast(double x_pos, 
              double y_pos, 
              int viewport_width,
              int viewport_height,
              mat4 projection,
              mat4 view,
              vec3 out)
{

    float x = (2.0 * x_pos) / viewport_width - 1.0f;
    float y = 1.0 - (2.0 * y_pos) / viewport_height;
    float z = 1.0;
    vec4 ray = {x, y, -z, 1.0};

    mat4 projection_inv;
    mat4 view_inv;
    mnf_mat4_inv(projection, projection_inv);
    mnf_mat4_inv(view, view_inv);
    mnf_mat4_mul_vec4(projection_inv, ray, ray);

    ray[2] = -1.0;
    ray[3] = 0.0;

    mnf_mat4_mul_vec4(view_inv, ray, ray);
    vec3 ray_world = {ray[0], ray[1], ray[2]};
    mnf_vec3_normalize(ray_world, ray_world);
    mnf_vec3_copy(ray_world, out);
}
*/
