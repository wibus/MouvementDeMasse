#version 120

uniform mat3 View;
uniform mat4 Projection;
uniform vec2 TexShift;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec3 fragDir;
varying vec2 texCoord;
varying vec3 position;


void main(void)
{
    fragDir     = View * position_att;
    position    = position_att;
    gl_Position = Projection * vec4(fragDir, 1.0);
    texCoord    = texCoord_att + TexShift;
}
