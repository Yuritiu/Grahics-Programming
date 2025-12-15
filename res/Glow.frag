#version 400

in vec3 vN;
in vec3 vPos;

out vec4 FragColour;

uniform vec3 glowColor;     // set from C++
uniform float glowPower;    // set from C++
uniform float glowStrength; // set from C++

void main()
{
    // Camera is effectively at origin in object space is not true,
    // but for a simple rim outline pass this works well enough visually.
    // If you want it physically correct, we can pass cameraPos + worldPos.
    vec3 N = normalize(vN);
    vec3 V = normalize(-vPos);

    float rim = 1.0 - max(dot(N, V), 0.0);
    rim = pow(rim, glowPower);

    FragColour = vec4(glowColor, rim * glowStrength);
}
