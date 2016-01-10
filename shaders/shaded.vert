#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec3 a_color;

varying vec3 v_normal;
varying vec3 v_color;

void main()
{
    gl_Position = mvp_matrix * a_position;

    v_normal = a_normal;
    v_color = a_color;

    /*vec3 light_pos = vec3(5);
    vec3 light_vector = light_pos - a_position.xyz;
    float attenuation = 1.0 / (0.1 * length(light_vector));
    light_vector = normalize(light_vector);
    float nx_dir = max(0.0, dot(a_normal, light_vector));
    vec3 light_diffuse = vec3(1, 1, 1);
    v_diffuse = light_diffuse * nx_dir * attenuation;*/
}








