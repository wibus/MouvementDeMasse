#version 120

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform vec3 Translation;

attribute vec3 position_att;
attribute vec3 normal_att;
attribute vec2 texCoord_att;

varying vec4 eyeVec;
varying vec3 normal;
varying vec2 texCoord;

void main(void)
{
    vec3 movedPos = position_att + Translation;
    eyeVec = ViewMatrix * vec4(movedPos, 1.0);
    gl_Position = ProjectionMatrix * eyeVec;

    normal = NormalMatrix * normal_att;
    texCoord = texCoord_att;
}
