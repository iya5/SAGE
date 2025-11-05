#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec2 attr_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 vert_uv;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);

    vert_uv = attr_uv;
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec2 vert_uv;

uniform sampler2D u_texture;

out vec4 frag_color;

void main()
{
    frag_color = texture(u_texture, vert_uv);
}

#endif /* COMPILE_FS */
