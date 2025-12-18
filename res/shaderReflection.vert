#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT {
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
} vs_out;

uniform mat4 transform; 
uniform mat4 model;

void main()
{
    vs_out.TexCoords = VertexTexCoord;

    vec4 world = model * vec4(VertexPosition, 1.0);
    vs_out.WorldPos = world.xyz;

    // basic normal transform
    vs_out.Normal = mat3(model) * VertexNormal;

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
