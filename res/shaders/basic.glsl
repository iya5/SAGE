#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec3 attr_normal;
layout (location = 2) in vec2 attr_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 frag_normal;
out vec2 frag_uv;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);

    frag_normal = attr_normal;
    frag_uv = attr_uv;
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec2 frag_uv;

uniform sampler2D u_texture;

out vec4 out_color;

void main()
{
    out_color = texture(u_texture, frag_uv);
}

#endif /* COMPILE_FS */
