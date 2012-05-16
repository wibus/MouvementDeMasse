#version 120

varying vec3 pos;

void main(void)
{
    float val = (pos.z + 1.0) / 2.0;
    vec4 color = vec4(val, val, val, 0.5);

    if(pos.x - floor(pos.x) < 0.1 ||
       pos.y - floor(pos.y) < 0.1)
        color.rgb = vec3(0.0, 0.0, 1.0);

    if(pos.z * 10 - floor(pos.z * 10) < 0.1)
        color.rgb = vec3(0.8, 0.8, 0.2);

    gl_FragColor = color;
}
