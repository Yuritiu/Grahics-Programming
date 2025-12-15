#version 400 core

in VS_OUT {
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuse; // you bind texture2 to this in C++
uniform vec3 cameraPos;    // set from C++ with setVec3("cameraPos", ...)

void main()
{
    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(cameraPos - fs_in.WorldPos);

    // Simple "environment-ish" effect using view angle:
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 3.0);

    vec3 baseColor = texture(diffuse, fs_in.TexCoords).rgb;
    vec3 highlight = vec3(1.0); // white reflection highlight

    vec3 finalColor = mix(baseColor, highlight, fresnel);

    FragColor = vec4(finalColor, 1.0);
}
