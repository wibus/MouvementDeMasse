#version 120

uniform mat3 View;
uniform mat4 Projection;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec3 position;
varying vec3 fragDir;


void main(void)
{
    position    = position_att;
    fragDir     = View * position;
    gl_Position = Projection * vec4(fragDir, 1.0);
}
