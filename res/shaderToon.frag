#version 400

in VS_OUT
{
    vec3 ObjPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform vec3 lightDir;

void main()
{
    // Toon lighting
    vec3 N = normalize(fs_in.Normal);
    vec3 L = normalize(lightDir);

    float intensity = max(dot(N, L), 0.0);
    float levels    = 3.0;
    float quantised = floor(intensity * levels) / levels;
    float toonLight = 0.3 + 0.7 * quantised;

    // Base colours 
    vec3 skinColor      = vec3(0.45, 0.30, 0.20); // brown
    vec3 eyeWhite       = vec3(0.96, 0.96, 0.96);
    vec3 eyeLeftColour  = vec3(0.2, 0.7, 1.0);    // blue
    vec3 eyeRightColour = vec3(0.2, 1.0, 0.4);    // green

    vec3 color = skinColor;

    // Eye regions as spheres in object space
    vec3 p = fs_in.ObjPos;

    // Approximate eye centres for standard Suzanne (object space)
    vec3 leftEyeCenter  = vec3(-0.35, 0.25, 0.68);
    vec3 rightEyeCenter = vec3( 0.35, 0.25, 0.68);

    float eyeRadius = 0.18;   // how big the coloured area is

    float dLeft  = length(p - leftEyeCenter);
    float dRight = length(p - rightEyeCenter);

    if (dLeft < eyeRadius)
    {
        // mix iris colour onto white
        color = mix(eyeWhite, eyeLeftColour, 0.5);
    }
    else if (dRight < eyeRadius)
    {
        color = mix(eyeWhite, eyeRightColour, 0.5);
    }

    // Apply toon lighting
    color *= toonLight;

    FragColor = vec4(color, 1.0);
}
