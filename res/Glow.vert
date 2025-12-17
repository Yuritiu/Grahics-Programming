#version 330 core
layout(location=0) in vec3 position;
layout(location=2) in vec3 normal;

uniform mat4 transform;   // MVP (your Shader::Update sets this)
uniform mat4 model;

out vec3 WorldPos;
out vec3 WorldNormal;

void main()
{
    vec4 wp = model * vec4(position, 1.0);
    WorldPos = wp.xyz;
    WorldNormal = mat3(transpose(inverse(model))) * normal;

    gl_Position = transform * vec4(position, 1.0);
}
