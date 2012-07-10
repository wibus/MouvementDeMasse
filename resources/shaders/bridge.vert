#version 120

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

attribute vec3 position_att;

void main(void)
{
    gl_Position = ProjectionMatrix * ModelViewMatrix * position_att;
}
