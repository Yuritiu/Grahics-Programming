#version 400

uniform float time;

in vec2 tC;

uniform sampler2D diffuse;
uniform sampler2D diffuse1;

out vec4 FragColour;

void main()
{
	if (sin(time)<0)
	{
	FragColour = texture2D(diffuse, tC);
	}
	else
		FragColour = texture2D(diffuse1, tC);
}
