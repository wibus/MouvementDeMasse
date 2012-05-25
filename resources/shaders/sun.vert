#version 120

uniform vec4 SunDirection;
uniform mat4 ModelView;
uniform mat4 Projection;

attribute vec2 relPos_att;

varying vec2 relPos_var;

void main(void)
{
    relPos_var = relPos_att;

    vec3 d = normalize(SunDirection.xyz);
    vec3 u = normalize(cross(d, vec3(0, 0 ,1)));
    vec3 v = normalize(cross(d, u));

    vec3 pos = relPos_att.s * u + relPos_att.t * v - d*200;
    vec4 translation = ModelView * vec4(0, 0, 0, 1);
    vec4 projPos = ModelView * vec4(pos, 1.0);
    gl_Position = Projection * vec4(projPos.xyz - translation.xyz, 1.0);
}
