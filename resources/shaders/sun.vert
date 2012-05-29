#version 120

uniform vec4 SunDirection;
uniform mat3 Rotation;
uniform mat4 Projection;
uniform float Radius;

attribute vec2 relPos_att;
varying vec2 relPos_var;

void main(void)
{
    relPos_var = relPos_att;

    vec3 d = normalize(SunDirection.xyz);
    vec3 u = normalize(cross(d, vec3(0, 0 ,1)));
    vec3 v = normalize(cross(d, u));

    vec3 pos = Rotation * ((relPos_att.s * u + relPos_att.t * v)*Radius - d*200);
    gl_Position = Projection * vec4(pos, 1.0);
}
