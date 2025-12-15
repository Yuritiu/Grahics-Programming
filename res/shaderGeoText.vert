#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

uniform mat4 transform;

void main()
{
    vs_out.Position = VertexPosition;        // object space
    vs_out.Normal   = normalize(VertexNormal);
    vs_out.TexCoord = VertexTexCoord;

    // still write gl_Position so the VS compiles cleanly
    gl_Position = transform * vec4(VertexPosition, 1.0);
}
