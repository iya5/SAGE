#ifdef COMPILE_VS

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

out vec4 out_color;

void main()
{
    out_color = vec4(1.0);
}

#endif /* COMPILE_FS */
