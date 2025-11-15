#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

uniform vec3 u_color;

out vec4 out_color;

void main()
{
    out_color = vec4(u_color, 1.0);
}

#endif /* COMPILE_FS */
