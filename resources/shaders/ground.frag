#version 120

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


varying vec3 norm;

varying vec4 eyeVec;
varying vec3 normal;
varying float height;
varying vec3 spotLightDir;


void main(void)
{
    vec4 color = texture1D(TextureUnit, (height+HillsAmplitude)/(2*HillsAmplitude));

    vec3 eyeN = normalize(eyeVec.xyz);
    vec3 lightN = normalize(-sun.direction.xyz);
    vec3 normalN = normalize(normal);

    vec3 totAmbient = vec3(0, 0, 0);
    vec3 totDiffuse = vec3(0, 0, 0);
    vec4 totSpecular = vec4(0, 0, 0 ,0);

    // Sun
    totAmbient  += sun.ambient;
    totDiffuse  += sun.diffuse * max(dot(normalN, lightN), 0.0);
    totSpecular += vec4(sun.specular, 1.0) * pow(max(dot(-eyeN, reflect(-lightN, normalN)), 0.0), Shininess);

    // Spots
    vec3 nSpotLightDir = normalize(spotLightDir);
    if(dot(-spots.direction, nSpotLightDir) > spots.cutoff)
    {
        totDiffuse  += spots.diffuse * max(dot(normalN, nSpotLightDir), 0.0);
        totSpecular += vec4(spots.specular, 1.0) * pow(max(dot(-eyeN, reflect(-nSpotLightDir, normalN)), 0.0), Shininess);
    }

    totSpecular *= (Shininess / 100.0);
    totSpecular = clamp(totSpecular, 0.0, 1.0);

    gl_FragColor = vec4((totAmbient + totDiffuse)*color.rgb + totSpecular.rgb,
                        max(color.a, totSpecular.a));
    //gl_FragColor = vec4(abs(norm.xy), 0 , 1);
}
