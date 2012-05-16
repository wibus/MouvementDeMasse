#version 120

uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;

attribute vec3 position_att;
attribute vec3 normal_att;

varying vec3 pos;

void main(void)
{
    pos = position_att;
    gl_Position = Projection * ModelView * vec4(position_att, 1.0);
}
