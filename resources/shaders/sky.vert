#version 120

uniform mat3 View;
uniform mat4 Projection;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec3 fragDir;
varying vec2 texCoord;

void main(void)
{
    fragDir     = View * position_att;
    gl_Position = Projection * vec4(fragDir, 1.0);
}
