#version 120

uniform mat3 View;
uniform mat4 Projection;
uniform float TexTShift;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec3 fragDir;
varying vec2 texCoord;
varying float positionZ;

void main(void)
{
    positionZ   = (1.0 - position_att.z);
    positionZ  *= positionZ * positionZ;

    fragDir     = View * position_att;
    gl_Position = Projection * vec4(fragDir, 1.0);
    texCoord    = vec2(texCoord_att.s, texCoord_att.t + TexTShift);
}
