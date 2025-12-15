#version 330 core

in VS_OUT
{
    vec3 WorldPos;
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture1;   // bound to GL_TEXTURE0 (noise.png)
uniform sampler2D texture2;   // bound to GL_TEXTURE1 (lava3.jpg)

uniform float time;
uniform float fogDensity;
uniform vec3  fogColor;
uniform float maxDist;
uniform float minDist;

void main()
{
    vec2 uv = fs_in.TexCoord;

    // Fake "noise" factor animated over time: value in [0,1]
    float n = 0.5 + 0.5 * sin(10.0 * uv.x + 4.0 * time) * cos(10.0 * uv.y - 4.0 * time);

    // Two textures: one slower, one faster / scaled
    vec3 col1 = texture(texture1, uv + vec2(time * 0.05, 0.0)).rgb;
    vec3 col2 = texture(texture2, uv * 2.0 + vec2(0.0, time * 0.05)).rgb;

    // Blend using animated "noise" factor – applies over the WHOLE surface
    vec3 lava = mix(col1, col2, n);

    // ----- Fog based on distance from origin -----
    float dist = length(fs_in.WorldPos);

    // Simple linear fog factor between minDist and maxDist
    float fogFactor = clamp((maxDist - dist) / (maxDist - minDist), 0.0, 1.0);

    // Make it a bit denser using fogDensity
    fogFactor = pow(fogFactor, fogDensity * 2.0);

    vec3 finalColor = mix(fogColor, lava, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}
