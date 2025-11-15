#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec3 attr_normal;
layout (location = 2) in vec2 attr_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_uv;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);

    frag_pos = attr_pos;
    frag_uv = attr_uv;
    frag_normal = attr_normal;
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

uniform sampler2D u_texture;
uniform vec4 u_color;

out vec4 out_color;

void main()
{
    //out_color = texture(u_texture, frag_uv) * u_color;
    out_color = u_color;
    //out_color = vec4(abs(frag_uv), 0.0, 1.0);
    //out_color = vec4(abs(frag_normal), 1.0);
    //out_color = vec4(abs(frag_pos), 1.0);
}

#endif /* COMPILE_FS */
