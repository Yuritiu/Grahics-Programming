#version 400 core

in VS_OUT 
{
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform vec3 cameraPos;

void main()
{
    // Base texture (Earth)
    vec3 base = texture(diffuse, fs_in.TexCoords).rgb;

    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(cameraPos - fs_in.WorldPos);
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 3.0);

    vec3 finalColor = mix(base, vec3(1.0), fresnel * 0.15); // keep subtle

    FragColor = vec4(finalColor, 1.0);
}
