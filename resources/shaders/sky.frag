#version 120

uniform vec4  SkyColor;
uniform vec4  SunColor;
uniform float SunRadius;
uniform vec4  SunPosition;
uniform sampler2D NightTexUnit;


varying vec3 fragDir;
varying vec2 texCoord;
varying float positionZ;

void main(void)
{
    vec4 color = SkyColor + positionZ * vec4(0.15);

    if(dot(normalize(fragDir), normalize(SunPosition.xyz)) > 0.999)
        color = SunColor;

    vec4 texColor = texture2D(NightTexUnit, texCoord);

    gl_FragColor = (texColor.a * texColor) + (1.0 - texColor.a) * color;
}
