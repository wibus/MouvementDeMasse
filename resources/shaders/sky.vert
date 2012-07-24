#version 120

uniform mat4 ProjectionMatrix;
uniform mat3 ViewMatrix;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec3 position;
varying vec3 fragDir;


void main(void)
{
    position    = position_att;
    fragDir     = ViewMatrix * position;
    gl_Position = ProjectionMatrix * vec4(fragDir, 1.0);
}
