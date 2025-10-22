#ifdef COMPILE_VS

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fs_uv;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    fs_uv = uv;
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec2 fs_uv;

uniform sampler2D sampler_texture;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

out vec4 out_color;

void main()
{
    out_color = texture(sampler_texture, fs_uv) * color;
}

#endif /* COMPILE_FS */
