#version 120

uniform sampler2D TextureUnit;

varying vec4 eyeVec;
varying vec3 normal;
varying vec2 texCoord;

void main(void)
{
    gl_FragColor = texture2D(TextureUnit, texCoord);
}
