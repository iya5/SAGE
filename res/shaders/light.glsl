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

out vec4 frag_color;

void main()
{
    frag_color = vec4(1.0);
}

#endif /* COMPILE_FS */
