#version 120

uniform vec4  NightSkyColor;
uniform vec4  DaySkyColor;
uniform vec4  SunSetColor;
uniform vec4  SkylineColor;
uniform vec4  SunColor;
uniform float SunRadius;
uniform vec4  VCSunPosition;
uniform vec4  ECSunPosition;
uniform float Clouds1Height = 0.2f;
uniform vec2  TexShift;
uniform float DayProgress;
uniform sampler2D CloudsTexUnit;
uniform sampler2D StarsTexUnit;
uniform mat4 SkyRotMatrix;

varying vec3 position;
varying vec3 fragDir;

const float SKY_DOWN_SHIFT = 0.12;
const float HORIZON_HEIGHT = 0.3;

void main(void)
{
    vec4 color;

    // Position
    vec3 pos = normalize(position);

    // Stars
    vec4 starsColor = NightSkyColor;
    if(VCSunPosition.z < HORIZON_HEIGHT)
    {
        vec3 skyCoord = vec3(SkyRotMatrix * vec4(pos, 1.0));
        vec2 texCoord = atan(vec2(skyCoord.y, skyCoord.z), vec2(skyCoord.x, length(skyCoord.xy))) / vec2(2*3.1416, 3.1416) + vec2(0.5, 0.5);
        starsColor = texture2D(StarsTexUnit, texCoord*vec2(2, 1));
    }

    // Background color
    float dayWeight = smoothstep(-HORIZON_HEIGHT, HORIZON_HEIGHT, VCSunPosition.z);
    color = mix(starsColor, DaySkyColor, dayWeight);
    float sunWeight = max(dot(VCSunPosition.xyz, pos), 0.0) * 0.5;
    float sunIntensity = pow(VCSunPosition.x*VCSunPosition.x + VCSunPosition.y*VCSunPosition.y, 10.0);
    color = mix(color, mix(NightSkyColor, SunSetColor, sunWeight), sunIntensity);

    // Skyline
    vec4 skylineColor = abs(pow(1.0 - pos.z, 6.0)) * SkylineColor;
    color += skylineColor;

    float sunItensity = clamp((dot(normalize(fragDir), ECSunPosition.xyz) - SunRadius)*25000.0,0.0, 1.0);
    color = sunItensity * SunColor + (1.0 - sunItensity) * color;

    if(pos.z > 0)
    {
        if(VCSunPosition.z > 0.0)
        {
            // Sun

        }

        // Clouds
        float ceilZ = pos.z+SKY_DOWN_SHIFT;
        vec2 ceilCoord = normalize(pos.xy) * length(pos.xy) / (ceilZ) + TexShift;
        vec4 cloudsColor = texture2D(CloudsTexUnit, ceilCoord * Clouds1Height);
        color = (cloudsColor.a * cloudsColor) + (1.0 - cloudsColor.a) * color;
    }

    gl_FragColor = color;
}
