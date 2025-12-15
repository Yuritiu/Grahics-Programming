#version 330 core

in vec3 gNormal;
in vec2 gTexCoord;

out vec4 FragColor;

uniform sampler2D diffuse;

// keep these so your C++ setFloat() calls still succeed
uniform float randColourX;
uniform float randColourY;
uniform float randColourZ;

void main()
{
    vec3 base = texture(diffuse, gTexCoord).rgb;

    vec3 N = normalize(gNormal);
    vec3 L = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(N, L), 0.2);

    FragColor = vec4(base * diff, 1.0);
}
