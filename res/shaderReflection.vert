#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT {
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
} vs_out;

// Combined MVP (this is what your Shader::Update usually sets)
uniform mat4 transform;
// Model matrix (you’re setting this from C++ in linkEmapping)
uniform mat4 model;

void main()
{
    // World position and normal
    vec4 worldPos   = model * vec4(VertexPosition, 1.0);
    vs_out.WorldPos = worldPos.xyz;

    // Transform normal into world space (basic version)
    vs_out.Normal   = mat3(model) * VertexNormal;

    vs_out.TexCoords = VertexTexCoord;

    // transform already includes view * projection * model
    gl_Position = transform * vec4(VertexPosition, 1.0);
}
