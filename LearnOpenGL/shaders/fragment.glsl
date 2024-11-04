#version 330 core
out vec4 FragColor;

in vec3 myColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// FragColor = texture(texture1, TexCoord) * vec4(myColor, 1.0f);
	// FragColor = texture(texture1, TexCoord);
	FragColor = mix(texture(texture1, TexCoord * vec2(1, -1)), texture(texture2, TexCoord), 0.5f);
}
