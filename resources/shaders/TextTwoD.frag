#version 120

uniform sampler2D tex;
uniform vec4 textColor = vec4(1.0);
varying vec2 texCoord;

void main(void)
{
    gl_FragColor = textColor * texture2D(tex, texCoord);
}
