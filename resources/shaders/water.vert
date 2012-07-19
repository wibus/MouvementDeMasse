#version 120

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;

attribute vec3 position_att;

varying vec4 eyeVec;
varying vec3 normal;

void main(void)
{
    eyeVec = ViewMatrix * vec4(position_att, 1.0);
    gl_Position = ProjectionMatrix * eyeVec;

    normal = NormalMatrix * vec3(0, 0, 1);
}
