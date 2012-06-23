#version 120

uniform mat4 ProjectionViewMatrix;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec2 texCoord;

void main(void)
{
    gl_Position = ProjectionViewMatrix * vec4(position_att, 1);

    texCoord = texCoord_att;
}
