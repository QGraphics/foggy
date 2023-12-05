#version 330 core

in vec2 TexCoord;

out vec3 frag_color;

uniform vec4 vertColor;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

void main()
{
	 //frag_color = vertColor;
	 vec4 tex1Col = texture(texSampler1, TexCoord);
	 vec4 tex2Col = texture(texSampler2, TexCoord);

	 frag_color = mix(tex1Col.rgb, tex2Col.rgb, tex2Col.a);
}