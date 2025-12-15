#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT {
    vec3 ObjPos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

uniform mat4 transform; // your Shader::Update sends this

void main()
{
    vs_out.ObjPos   = VertexPosition;
    vs_out.Normal   = VertexNormal;
    vs_out.TexCoord = VertexTexCoord;

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
