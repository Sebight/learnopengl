#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;

void main()
{
	float blend = (sin(time*3) / 2) + 0.5f;
	FragColor = mix(texture(texture1, TexCoord * vec2(1, -1)), texture(texture2, TexCoord), blend);
}
