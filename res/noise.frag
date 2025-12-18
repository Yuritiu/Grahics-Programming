#version 400

uniform sampler2D texture2;

uniform float maxDist;
uniform float minDist;
uniform float fogDensity;
uniform vec3 fogColor;

in vec3 vObjPos;
in vec3 vObjN;

out vec4 FragColour;

vec2 rot90(vec2 uv) { return vec2(-uv.y, uv.x); }

vec3 triplanar(sampler2D tex, vec3 pos, vec3 n, float scale)
{
    // Very soft blending
    vec3 w = abs(n);
    w = w / (w.x + w.y + w.z + 1e-6);

    vec2 uvX = pos.zy * scale; // projection for faces pointing X
    vec2 uvY = pos.xz * scale; // projection for faces pointing Y
    vec2 uvZ = pos.xy * scale; // projection for faces pointing Z

    // Rotate two projections so all three “flow” similarly
    uvY = rot90(uvY);
    uvZ = rot90(uvZ);

    vec3 x = texture(tex, uvX).rgb;
    vec3 y = texture(tex, uvY).rgb;
    vec3 z = texture(tex, uvZ).rgb;

    return x * w.x + y * w.y + z * w.z;
}

void main()
{
    vec3 base = triplanar(texture2, vObjPos, normalize(vObjN), 0.08);

    // Depth-based fog 
    float d = clamp(gl_FragCoord.z, 0.0, 1.0);

    float fogFactor = clamp((maxDist - d) / (maxDist - minDist), 0.0, 1.0);
    fogFactor = pow(fogFactor, fogDensity * 2.0);

    vec3 finalColour = mix(fogColor, base, fogFactor);
    FragColour = vec4(finalColour, 1.0);
}
