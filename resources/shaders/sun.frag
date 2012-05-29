#version 120

uniform vec4 Color;
uniform float Radius;

varying vec2 relPos_var;

void main(void)
{
    float dist = distance(vec2(0, 0), relPos_var);
    gl_FragColor = vec4(Color.rgb, Color.a * pow(max(1-dist, 0), 0.1));
}
