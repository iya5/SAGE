#version 460 core
out vec4 frag_color;

uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

in vec3 fs_normal;
in vec3 fragment_position;

void main()
{
    vec3 normal = normalize(fs_normal);
    vec3 light_direction =  normalize(light_pos - fragment_position);

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    float specular_intensity = 0.5;
    vec3 view_direction = normalize(view_pos - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, normal);

    float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
    vec3 specular = specular_intensity * spec * light_color;

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;
    vec3 result = (ambient + diffuse + specular) * object_color;
    frag_color = vec4(result, 1.0);
}
