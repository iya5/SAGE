#version 410 core
out vec4 frag_color;

in vec2 fs_uv;

uniform sampler2D sampler_texture;

void main()
{
    frag_color = texture(sampler_texture, fs_uv);
}
