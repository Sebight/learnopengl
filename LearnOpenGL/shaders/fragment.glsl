#version 330 core
out vec4 FragColor;

in vec3 myColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	// FragColor = texture(ourTexture, TexCoord) * vec4(myColor, 1.0f);
	FragColor = texture(ourTexture, TexCoord);
}