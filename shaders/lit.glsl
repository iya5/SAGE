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

uniform vec3 object_color;
uniform vec3 light_color;
uniform sampler2D sampler_texture;

out vec4 out_color;

void main()
{
    vec4 result_color = vec4(light_color * object_color, 1.0);
    out_color = texture(sampler_texture, fs_uv) * result_color;
}

#endif /* COMPILE_FS */
