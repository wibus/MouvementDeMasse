#version 120

uniform sampler2D TextureUnit;
uniform float StoryHeight;

varying vec2 texCoord;

void main(void)
{
    float tCoord = texCoord.t - floor(texCoord.t);
    if(texCoord.t > 1.0 && tCoord < 0.5)
        tCoord += 0.5;

    gl_FragColor = texture2D(TextureUnit, vec2(texCoord.s, tCoord));
}
