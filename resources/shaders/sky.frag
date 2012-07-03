#version 120

uniform vec4  SkyColor;
uniform vec4  SkylineColor;
uniform vec4  SunColor;
uniform float SunRadius;
uniform vec4  SunPosition;
uniform float Cloudyness;
uniform float CloudsTightness;
uniform sampler2D CloudsTexUnit;

varying vec3 fragDir;
varying vec2 texCoord;
varying vec3 position;


void main(void)
{
    vec4 color = SkyColor;

    // Skyline
    vec4 skylineColor = abs(pow(1.0 - position.z, 6.0)) * SkylineColor;
    color += skylineColor;

    // Sun
    float sunItensity = clamp((dot(normalize(fragDir), SunPosition.xyz) - 0.99994)*100000.0,0.0, 1.0);
    color = sunItensity * SunColor + (1.0 - sunItensity) * color;

    // Clouds
    vec4 cloudsColor = texture2D(CloudsTexUnit, texCoord);
    cloudsColor.a = pow(clamp((Cloudyness-cloudsColor.a), 0.0, 1.0), CloudsTightness);
    color = (cloudsColor.a * cloudsColor) + (1.0 - cloudsColor.a) * color;

    gl_FragColor = color;
}
