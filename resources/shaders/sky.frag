#version 120

uniform vec4  SkyColor;
uniform vec4  SunColor;
uniform float SunRadius;
uniform vec3  SunPosition;


varying vec3 fragDir;
varying vec2 texCoord;

void main(void)
{
    vec4 color = SkyColor;

    if(dot(normalize(fragDir), normalize(SunPosition)) > 0.999)
        color = SunColor;

    gl_FragColor = color;
}
