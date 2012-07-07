#version 120

uniform vec4  SkyColor;
uniform vec4  SkylineColor;
uniform vec4  SunColor;
uniform float SunRadius;
uniform vec4  SunPosition;
uniform float Clouds1Height = 0.2f;
uniform vec2  TexShift;
uniform sampler2D CloudsTexUnit;

varying vec3 position;
varying vec3 fragDir;


void main(void)
{
    vec4 color = SkyColor;

    // Position
    vec3 pos = normalize(position);
    vec2 ceilCoord = pos.xy * length(pos.xy) / (pos.z+0.1) + TexShift;

    // Skyline
    vec4 skylineColor = abs(pow(1.0 - position.z, 6.0)) * SkylineColor;
    color += skylineColor;

    // Sun
    float sunItensity = clamp((dot(normalize(fragDir), SunPosition.xyz) - SunRadius)*25000.0,0.0, 1.0);
    color = sunItensity * SunColor + (1.0 - sunItensity) * color;

    // Clouds
    vec4 cloudsColor = texture2D(CloudsTexUnit, ceilCoord * Clouds1Height);
    color = (cloudsColor.a * cloudsColor) + (1.0 - cloudsColor.a) * color;

    gl_FragColor = color;
}
