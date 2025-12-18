#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextCoords;

// send uv to fragment shader
out vec2 tC;

uniform mat4 transform;

void main()
{
	tC = TextCoords;
	gl_Position = transform * vec4(VertexPosition, 1.0);
}