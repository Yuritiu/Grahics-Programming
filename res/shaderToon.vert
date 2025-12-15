#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT
{
    vec3 ObjPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 transform;

void main()
{
    vs_out.ObjPos    = VertexPosition;   // object-space position
    vs_out.Normal    = VertexNormal;
    vs_out.TexCoords = VertexTexCoord;

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
