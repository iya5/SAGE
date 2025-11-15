#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 frag_uv;

void main()
{
    gl_Position = u_projection * u_view * vec4(attr_pos, 1.0);
    frag_uv = attr_pos;
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec3 frag_uv;

uniform samplerCube u_skybox;

out vec4 out_color;

void main()
{
    out_color = texture(u_skybox, frag_uv);
}

#endif /* COMPILE_FS */
