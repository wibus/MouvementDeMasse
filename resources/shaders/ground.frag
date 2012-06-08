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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 direction;
};

struct SpotLight
{
    vec3 attCoefs;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 position;
    vec3 direction;
    float cutoff;
};

uniform DirectionnalLight sun;
uniform SpotLight spots;
uniform sampler1D TextureUnit;
uniform float HillsAmplitude;
uniform float Shininess;

varying vec4 eyeVec;
varying vec3 normal;
varying float height;
varying vec3 spotLightDir;
varying float spotDist;

vec4 NormalizedBlinnPhong(in vec3 N, in vec3 L, in vec3 V,
                          in Material mat, in vec4 lightDiff, in vec4 lightSpec)
{
    vec3  H = normalize(L + V);
    float specInt = mat.shininess * mat.fresnel * pow(max(dot(reflect(-L,N), V), 0.0), mat.shininess);
    return (lightDiff * mat.diffuse + lightSpec * mat.specular*specInt) * max(dot(N, L), 0.0) / 3.1416;
}

void main(void)
{
    vec4 color = texture1D(TextureUnit, (height+HillsAmplitude)/(2*HillsAmplitude));
    Material mat = Material(color, vec4(1), Shininess, 0.05);

    vec3 V = normalize(-eyeVec.xyz);
    vec3 L = normalize(-sun.direction.xyz);
    vec3 N = normalize(normal);

    vec4 finalColor = vec4(0, 0, 0, 0);

    // Sun
    finalColor += vec4(sun.ambient,1) * color;
    finalColor += NormalizedBlinnPhong(N, L, V, mat, vec4(sun.diffuse,1), vec4(sun.specular,1));

    // Spots
    vec3 S = normalize(spotLightDir);
    if(dot(-spots.direction, S) > spots.cutoff)
    {
        float att = 1.0 / (spots.attCoefs.x + (spots.attCoefs.y +(spots.attCoefs.z*spotDist)*spotDist));
        finalColor += att * NormalizedBlinnPhong(N, S, V, mat, vec4(spots.diffuse,1), vec4(spots.specular,1));
    }

    gl_FragColor = vec4(finalColor.rgb, max(color.a, finalColor.a));
}
