#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec2 attr_uv;
layout (location = 2) in vec3 attr_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 vert_uv;
out vec3 vert_normal;
out vec3 vert_frag_pos;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);

    vert_uv = attr_uv;
    /* this is very costly, it's much better to do it on the CPU first */
    vert_normal = mat3(transpose(inverse(u_model))) * attr_normal;
    vert_frag_pos = vec3(u_model * vec4(attr_pos, 1.0));
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec2 vert_uv;
in vec3 vert_normal;
in vec3 vert_frag_pos;


struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_view_pos;
uniform sampler2D u_texture;

uniform material u_material;
uniform light u_light;

out vec4 frag_color;

void main()
{
    //ambient
    vec3 ambient = u_light.ambient * u_material.ambient;

    // diffuse
    vec3 normal = normalize(vert_normal);
    vec3 light_direction = normalize(u_light.pos - vert_frag_pos);
    // we max it to zero because it means the light is behind the object's face
    float diffuse_scalar = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = u_light.diffuse * (diffuse_scalar * u_material.diffuse);

    // specular
    vec3 view_direction = normalize(u_view_pos - vert_frag_pos);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_scalar = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * (specular_scalar * u_material.specular);

    vec3 result_color = ambient + diffuse + specular;
    frag_color = texture(u_texture, vert_uv) * vec4(result_color, 1.0);
}

#endif /* COMPILE_FS */
