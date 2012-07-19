#version 120

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix = mat4(1.0);

attribute vec3 position_att;
attribute vec4 color_att;

varying vec4 color;

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position_att, 1);
    color = color_att;
}
