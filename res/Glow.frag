#version 330 core
in vec3 WorldPos;
in vec3 WorldNormal;

out vec4 FragColor;

uniform vec3  cameraPos;

uniform vec3  glowInnerColor;
uniform vec3  glowOuterColor;
uniform float glowStrength;
uniform float glowPower;
uniform float rimStart;
uniform float rimEnd;

void main()
{
    vec3 N = normalize(WorldNormal);
    vec3 V = normalize(cameraPos - WorldPos);

    float rim = 1.0 - clamp(dot(N, V), 0.0, 1.0);
    float soft = smoothstep(rimStart, rimEnd, rim);
    soft = pow(soft, glowPower);

    vec3 col = mix(glowOuterColor, glowInnerColor, soft);
    float a = soft * glowStrength;

    FragColor = vec4(col * a, a);
}
