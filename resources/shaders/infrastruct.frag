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
uniform vec2 RepeatFrom;
uniform sampler2D TexUnit;
uniform sampler2D SpecUnit;

varying vec4 eyeVec;
varying vec3 normal;
varying vec2 texCoord;


vec4 NormalizedBlinnPhong(in vec3 N, in vec3 L, in vec3 V,
                          in Material mat, in vec4 lightDiff, in vec4 lightSpec)
{
    vec3 H = normalize(L+V);
    float specInt = mat.shininess * mat.fresnel * pow(max(dot(H, N), 0.0), mat.shininess);
    return (lightDiff * mat.diffuse + lightSpec * mat.specular*specInt) * max(dot(N, L), 0.0) / 3.1416;
}

void main(void)
{
    vec2 tex = texCoord - floor(texCoord);
    if(abs(texCoord.s) > 1.0 && tex.s < 0.5)
        tex.s += RepeatFrom.s;
    if(abs(texCoord.t) > 1.0 && tex.t < 0.5)
        tex.t += RepeatFrom.t;

    vec4 color = texture2D(TexUnit, tex);
    vec4 specular = texture2D(SpecUnit, tex);
    Material mat = Material(color, specular, Shininess, 0.05);

    vec3 V = normalize(-eyeVec.xyz);
    vec3 L = normalize(-sun.direction.xyz);
    vec3 N = normalize(normal);

    vec4 finalColor = sun.ambient * color +
            NormalizedBlinnPhong(N, L, V, mat, sun.diffuse, sun.specular);

    gl_FragColor = vec4(finalColor.rgb, max(color.a, finalColor.a));
}
