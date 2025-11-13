#ifdef COMPILE_VS

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec3 attr_normal;
layout (location = 2) in vec2 attr_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
/* uniform mat3 u_normal_matrix; */

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_uv;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(attr_pos, 1.0);


    /* This is very costly, it's much better to do it on the CPU first. The
       for doing this is because we want to account for any transformations done
       to the vertex by the model matrix, thus the normal matrix is simply the
       inverse of the model matrix to remove any discreptencies then transposing
       it or getting the upper-left 3x3 to remove the translation part.
      
       NOTE: I said model because we're representing the vertex in world space,
       but we can optimize by also converting this in view space to reduce one
       variable needed (view position) for calculating specular */
    mat3 normal_mat = mat3(transpose(inverse(u_model)));
    frag_pos = vec3(u_model * vec4(attr_pos, 1.0));
    frag_normal = normal_mat * attr_normal;
    frag_uv = attr_uv;
}

#endif /* COMPILE_VS */

#ifdef COMPILE_FS

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

struct material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform material u_material;

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
#define MAX_SCENE_POINT_LIGHT 8
uniform int u_num_point_lights;
uniform point_light u_point_lights[MAX_SCENE_POINT_LIGHT];

uniform vec3 u_view_pos;
uniform sampler2D u_texture;

out vec4 out_color;

vec3 point_light_calculate(point_light light, vec3 normal, vec3 view_direction);
vec3 directional_light_calculate(directional_light light);

vec3 point_light_calculate(point_light light, vec3 normal, vec3 view_direction)
{
    /* Retrieve the light direction by getting the difference between its
       position and the fragment currently being shaded. This will be used for
       both the diffuse & specular reflection model */
    vec3 light_direction = normalize(light.pos - frag_pos);

    /* Calculating attenuation so that the light does not extend infinitely. 
       It's the light's brightness with respect to the distance of
       objects being shaded

       There are 4 real parameters for calculating attenuation, it is:
            - Range
            - Constant
            - Linear
            - Quadratic

        Range simply discards calculating the light's effect on the fragment.
        The result is a scalar representing the light's reach on the fragment

        Reference: Ogre Wiki - Point Light Attenuation */
    float distance = length(light.pos - frag_pos);
    float attenuation = light.constant + light.linear * distance + 
                        light.quadratic * (distance * distance);
    attenuation = 1 / attenuation;

    /* Ambient is a simple way of modeling global illumination without having
       to do any expensive calculation by assuming that there's some constant
       light being applied to every object. This can be interpreted as light
       from the moon, stars, or somewhere else

       Calculating it is very easy by simply multiplying 2 constants from the
       lighting equation, the material's color, & the light's ambience */
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, frag_uv));

    /* Diffuse reflection, also known as Lambertian reflection, based on
       Lambert's cosine law simply calculates how much light is a particular
       face (if implemented in such a way-it can also be done by calculating
       vertex normals instead) receives based on the angle of the light, and the
       surface normal. 

       This is simply the cosine angle between them, which can be retrieved by
       using the dot product, and clamping it at 0 if the angle between the
       light source & the face is greater than 90 degrees (no such thing as
       negative light).

       The result produces a matte-looking material (non-glossy), or the ideal
       diffuse reflection */
    float diffuse_factor = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diffuse_factor * vec3(texture(u_material.diffuse, frag_uv));

    /* Specular reflection mimics how we can see the reflection of light based
       on our viewing angle of the surface & the position of the light, as well
       as other factors relating to the material such as its shininess, & etc.

       Specular is calculated by reflecting the light direction about the normal
       and calculating the dot product between it & the view direction. If both
       vectors are aligned, it means the specular component (or the brightness
       of the reflection is being fully reflected)

       This is what produces a glossy-look to materials, & when combined, gives
       the Phong reflection model
            - The reflected light = ambient + diffuse + specular

        The specular also contains a phong exponent that controls the shininess
        of the material */
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_factor = pow(max(dot(reflect_direction, view_direction), 0.0), u_material.shininess);
    vec3 specular = light.specular * specular_factor * vec3(texture(u_material.specular, frag_uv));

    /* Applying the light's luminostiy (strength) based on the attenuation
       factors so that the brightness of the light on the fragment decreases
       as distance increases.so intensity of light on the fragment decreases as
       distance of the fragment to the light increases */
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    /* Combines all the lighting components to a single RGB 3-component vector */
    return (ambient + diffuse + specular);
}

/* This is the same function as above (calculating point lights) with the
   exception that the light is treated as a direction without a position.
   Conceptually thought of as being infinitely far away with the light direction
   all being parallel, hence every object is lit the same way */
vec3 directional_light_calculate(directional_light light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);

    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, frag_uv));

    float diffuse_factor = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diffuse_factor * vec3(texture(u_material.diffuse, frag_uv));

    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_factor = pow(max(dot(reflect_direction, view_direction), 0.0), u_material.shininess);
    vec3 specular = light.specular * specular_factor * vec3(texture(u_material.specular, frag_uv));

    return (ambient + diffuse + specular);
}

void main()
{
    /* Calculating the light based on multiple light casters of a fragment is
       simply done by calculating the resulting influence of each of those
       lights in a scene */
    vec3 output_color = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(frag_normal);
    vec3 view_direction = normalize(u_view_pos - frag_pos);

    /* Calculating the influence of the environment light (sun?) */
    output_color += directional_light_calculate(u_directional_light, normal, view_direction);

    /* Calculating the influence of all point lights passed to the shader */
    for (int i = 0; i < u_num_point_lights; i++)
        output_color += point_light_calculate(u_point_lights[i], normal, view_direction);

    out_color = vec4(output_color, 1.0);
}

#endif /* COMPILE_FS */
