#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out vec3 vN;
out vec3 vPos;

uniform mat4 transform;

void main()
{
    vPos = VertexPosition;             // object-space
    vN   = normalize(VertexNormal);

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
