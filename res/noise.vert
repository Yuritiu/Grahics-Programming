#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out vec3 vObjPos;
out vec3 vObjN;

uniform mat4 transform; 

void main()
{
    vObjPos = VertexPosition; // object space (camera-independent)
    vObjN = normalize(VertexPosition);

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
