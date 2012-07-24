#version 120

uniform vec4 Color = vec4(1.0, 1.0, 1.0, 1.0);

varying vec4 color;

void main(void)
{
    gl_FragColor = color;

}
