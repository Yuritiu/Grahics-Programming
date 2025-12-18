#version 330 core
in vec3 WorldPos;
in vec3 WorldNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube dynamicCube;
uniform float reflectStrength;

void main()
{
    vec3 N = normalize(WorldNormal);
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = reflect(I, N);

    vec3 env = textureLod(dynamicCube, R, 0.0).rgb;

    // mirror like appearance
    FragColor = vec4(env * reflectStrength, 1.0);
}
