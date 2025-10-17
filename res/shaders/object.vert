#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragment_position;
out vec3 fs_normal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    //fs_normal = normal;
    fs_normal = mat3(transpose(inverse(model))) * normal;

    fragment_position = vec3(model * vec4(position, 1.0));
}
