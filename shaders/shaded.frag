#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec3 v_normal;
varying vec3 v_color;

void main()
{
    vec3 light_ambient = vec3(1) * 0.2;

    vec3 light_vector = normalize(vec3(1, 2, 3));
    vec3 normal = normalize(v_normal);
    float nx_dir = max(0.0, dot(normal, light_vector));
    float nx_dir_secondary = -min(0.0, dot(normal, light_vector)) * 0.4;
    vec3 light_diffuse = vec3(1);
    vec3 diffuse = light_diffuse * (nx_dir + nx_dir_secondary);

    gl_FragColor = vec4((light_ambient + diffuse) * v_color, 1);
}
