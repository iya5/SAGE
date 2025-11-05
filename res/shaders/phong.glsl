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

struct light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform material u_material;
uniform light u_light;
uniform int u_flat_shading;
uniform vec3 u_view_pos;
uniform sampler2D u_texture;

out vec4 frag_color;

void main()
{
    /* determine the type of light */
    vec3 light_direction;
    if (u_flat_shading == 1) {
        light_direction = normalize(u_light.pos - vert_frag_pos);
    } else {
        light_direction = normalize(-u_light.pos);
    }

    /* Ambient:
     *      ka*ki
     */
    vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, vert_uv));

    /* Diffuse:
     *      kd * (Lm * N) * imd
     */
    vec3 normal = normalize(vert_normal);


    /* Max at zero since Lm is behind the surface */
    float diffuse_scalar = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = u_light.diffuse * diffuse_scalar * vec3(texture(u_material.diffuse, vert_uv));

    /* Specular:
     *      ks * (Rm + V)^a * ims
     * Where Rm is calculated as:
     *      Rm = 2 * (Lm dot N) * N - Lm
     *
     * GLSL provides a function for reflection a vector across a normal:
     * reflect(vec3 v, vec3 normal) and it returns a reflected vector.
     */
    vec3 view_direction = normalize(u_view_pos - vert_frag_pos);
    /* vec3 reflect_direction = reflect(-light_direction, normal); */
    vec3 reflect_direction = (2 * dot(light_direction, normal) * normal) - light_direction;
    float specular_scalar = pow(max(dot(reflect_direction, view_direction), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * specular_scalar * vec3(texture(u_material.specular, vert_uv));

    vec3 result_color = ambient + diffuse + specular;
    frag_color = texture(u_texture, vert_uv) * vec4(result_color, 1.0);
}

#endif /* COMPILE_FS */
