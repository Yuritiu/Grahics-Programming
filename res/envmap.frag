#version 330 core
in vec3 WorldPos;
in vec3 WorldNormal;
in vec2 UV;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D diffuse;     // earth.jpg
uniform float reflectStrength; // 0..1 (tune)

void main(){
    vec3 N = normalize(WorldNormal);

    // base earth color
    vec3 base = texture(diffuse, UV).rgb;

    // reflection
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = reflect(I, N);
    vec3 env = texture(skybox, R).rgb;

    // fresnel makes reflection stronger at grazing angles (looks nicer)
    float fresnel = pow(1.0 - max(dot(-I, N), 0.0), 5.0);

    float k = clamp(reflectStrength * fresnel, 0.0, 1.0);
    vec3 color = mix(base, env, k);

    FragColor = vec4(color, 1.0);
}
