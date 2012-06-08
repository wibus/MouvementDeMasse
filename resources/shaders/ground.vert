#version 120

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

uniform SpotLight spots;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

attribute vec3 position_att;
attribute vec3 normal_att;

varying vec4 eyeVec;
varying vec3 normal;
varying float height;
varying vec3 spotLightDir;
varying float spotDist;

void main(void)
{
    eyeVec = ModelViewMatrix * vec4(position_att, 1.0);
    gl_Position = ProjectionMatrix * eyeVec;

    normal = NormalMatrix * normal_att;
    height = position_att.z;
    spotLightDir= (spots.position - eyeVec).xyz;
    spotDist = length(eyeVec);
}
