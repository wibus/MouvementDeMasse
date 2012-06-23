uniform mat4 ProjectionViewMatrix;
uniform vec4 Translation;

attribute vec3 position_att;
attribute vec2 texCoord_att;

varying vec2 texCoord;

void main(void)
{
    float height = position_att.z * Translation.w + Translation.z;
    gl_Position = ProjectionViewMatrix * vec4(position_att.xy + Translation.xy, height, 1);
    texCoord = vec2(texCoord_att.s, texCoord_att.t*Translation.w);
}
