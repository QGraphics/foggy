#version 330 core
out vec4 frag_color;
in vec2 TexCoord;

uniform vec4 vertColor;
uniform sampler2D texSampler;

void main()
{
	frag_color = texture(texSampler, TexCoord);
}