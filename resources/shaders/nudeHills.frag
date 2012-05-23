#version 120

struct DirectionnalLight
{
    vec4 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionnalLight sun;

varying vec4 eyeVec;
varying vec3 normal;
varying vec4 color;

void main(void)
{
    vec3 eyeN = normalize(eyeVec.xyz);
    vec3 lightN = normalize(-sun.direction.xyz);
    vec3 normalN = normalize(normal);

    vec3 ambient  = sun.ambient;
    vec3 diffuse  = sun.diffuse * max(dot(normalN, lightN), 0.0);
    vec4 specular = vec4(sun.specular, 1.0) * pow(max(dot(-eyeN, reflect(-lightN, normalN)), 0.0), 100);

    gl_FragColor = vec4((ambient + diffuse)*color.rgb + specular.rgb, max(color.a, specular.a));
}
