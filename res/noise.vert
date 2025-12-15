#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT
{
    vec3 WorldPos;
    vec2 TexCoord;
} vs_out;

uniform mat4 transform;   // set from C++ via noiseShader.Update

void main()
{
    vec4 worldPos = transform * vec4(VertexPosition, 1.0);

    vs_out.WorldPos = worldPos.xyz;
    vs_out.TexCoord = VertexTexCoord;

    gl_Position = worldPos;
}
