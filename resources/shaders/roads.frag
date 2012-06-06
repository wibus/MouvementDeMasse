#version 120

varying vec4 eyeVec;
varying vec3 normal;
varying vec2 texCoord;

void main(void)
{
    gl_FragColor = vec4(normal, 1);
}
