#version 120

struct Material
{
    vec4  diffuse;
    vec4  specular;
    float shininess;
    float fresnel;
};

struct DirectionnalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 direction;
};

uniform DirectionnalLight sun;
uniform float Shininess;
uniform vec4  WaterColor;

varying vec4 eyeVec;
varying vec3 normal;


vec4 NormalizedBlinnPhong(in vec3 N, in vec3 L, in vec3 V,
                          in Material mat, in vec4 lightDiff, in vec4 lightSpec)
{
    vec3  H = normalize(L + V);
    float specInt = mat.shininess * mat.fresnel * pow(max(dot(reflect(-L,N), V), 0.0), mat.shininess);
    return (lightDiff * mat.diffuse + lightSpec * mat.specular*specInt) * max(dot(N, L), 0.0) / 3.1416;
}

void main(void)
{
    Material mat = Material(WaterColor, vec4(1), Shininess, 0.05);

    vec3 V = normalize(-eyeVec.xyz);
    vec3 L = normalize(-sun.direction.xyz);
    vec3 N = normalize(normal);

    vec4 finalColor = vec4(0, 0, 0, 0);

    // Sun
    finalColor += sun.ambient * WaterColor;
    finalColor += NormalizedBlinnPhong(N, L, V, mat, sun.diffuse, sun.specular);

    gl_FragColor = vec4(finalColor.rgb, max(WaterColor.a, finalColor.a));
}
