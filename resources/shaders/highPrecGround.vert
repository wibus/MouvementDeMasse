#version 120

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

attribute vec3 position_att;

void main(void)
{
    gl_PointSize = 2.0f;
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(position_att, 1);
}
