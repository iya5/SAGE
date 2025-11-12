#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec2 attr_uv;
layout (location = 2) in vec3 attr_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
/* uniform mat3 u_normal_matrix; */

out vec2 vert_uv;
out vec3 vert_normal;
out vec3 vert_frag_pos;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);

    vert_uv = attr_uv;

    /* 
     * this is very costly, it's much better to do it on the CPU first. The
     * for doing this is because we want to account for any transformations done
     * to the vertex by the model matrix, thus the normal matrix is simply the
     * inverse of the model matrix to remove any discreptencies then transposing
     * it or getting the upper-left 3x3 to remove the translation part.
     *
     * NOTE: I said model because we're representing the vertex in world space,
     * but we can optimize by also converting this in view space to reduce one
     * variable needed (view position) for calculating specular.
     */
    mat3 normal_mat = mat3(transpose(inverse(u_model)));
    vert_normal = normal_mat * attr_normal;
    vert_frag_pos = vec3(u_model * vec4(attr_pos, 1.0));
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec2 vert_uv;
in vec3 vert_normal;
in vec3 vert_frag_pos;

struct material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

/* This is almost like the sun */
struct directional_light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform directional_light u_directional_light;

struct point_light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define N_POINT_LIGHT 1
uniform point_light u_point_lights[N_POINT_LIGHT];

uniform material u_material;
uniform vec3 u_view_pos;
uniform sampler2D u_texture;

out vec4 frag_color;

vec3 point_light_calculate(point_light light, vec3 normal, vec3 view_direction);
vec3 directional_light_calculate(directional_light light);

vec3 point_light_calculate(point_light light, vec3 normal, vec3 view_direction)
{
    vec3 result_color = vec3(0.0, 0.0, 0.0);

    /* determine the type of light */
    vec3 light_direction = normalize(light.pos - vert_frag_pos);

    /* attenuation */
    float distance = length(light.pos - vert_frag_pos);
    float attenuation = 1.0 / (light.constant
        + light.linear
        * distance
        + light.quadratic
        * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, vert_uv));

    float diffuse_scalar = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diffuse_scalar * vec3(texture(u_material.diffuse, vert_uv));

    vec3 reflect_direction = reflect(-light_direction, normal);
    //vec3 reflect_direction = (2 * dot(light_direction, normal) * normal) - light_direction;
    float specular_scalar = pow(max(dot(reflect_direction, view_direction), 0.0), u_material.shininess);
    vec3 specular = light.specular * specular_scalar * vec3(texture(u_material.specular, vert_uv));

    /* applying attenuatation */
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 directional_light_calculate(directional_light light, vec3 normal, vec3 view_direction)
{
    /* normalizing just in case */
    vec3 light_direction = normalize(-light.direction);

    vec3 result_color = vec3(0.0, 0.0, 0.0);

    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, vert_uv));

    float diffuse_scalar = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diffuse_scalar * vec3(texture(u_material.diffuse, vert_uv));

    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_scalar = pow(max(dot(reflect_direction, view_direction), 0.0), u_material.shininess);
    vec3 specular = light.specular * specular_scalar * vec3(texture(u_material.specular, vert_uv));

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 output_color = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(vert_normal);
    vec3 view_direction = normalize(u_view_pos - vert_frag_pos);

    output_color += directional_light_calculate(u_directional_light, normal, view_direction);

    for (int i = 0; i < N_POINT_LIGHT; i++) {
        output_color += point_light_calculate(u_point_lights[i], normal, view_direction);
    }

    frag_color = vec4(output_color, 1.0);
}

#endif /* COMPILE_FS */
