#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 ObjPos;
    vec3 Normal;
    vec2 TexCoord;
} gs_in[];

out vec3 gNormal;
out vec2 gTexCoord;

uniform mat4 transform;
uniform float time;

vec3 explode(vec3 pos, vec3 normal)
{
    float strength = 0.5 * sin(time * 2.0) + 0.5; // 0..1 pulse
    float magnitude = 0.15;                       // tweak if needed
    return pos + normalize(normal) * strength * magnitude;
}

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gNormal   = gs_in[i].Normal;
        gTexCoord = gs_in[i].TexCoord;

        vec3 p = explode(gs_in[i].ObjPos, gs_in[i].Normal);
        gl_Position = transform * vec4(p, 1.0);

        EmitVertex();
    }
    EndPrimitive();
}
