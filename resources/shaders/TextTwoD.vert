#version 120

uniform mat4 ModelViewProjectionMatrix;
varying vec2 texCoord;

void main(void)
{
    texCoord = gl_MultiTexCoord0.st;
    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
}
