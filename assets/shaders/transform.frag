#version 330 core
out vec4 frag_color;
in vec2 TexCoord;

uniform vec4 vertColor;
uniform sampler2D texSampler;
uniform sampler2D texSampler1;

void main()
{
//    vec4 texColor = texture(texSampler, TexCoord);
//    vec4 texColor1 = texture(texSampler1, TexCoord);
//    frag_color = mix(texColor.rgb, texColor1.rgb, texColor1.a);
        frag_color = texture(texSampler1, TexCoord);
//    frag_color = mix(texColor.rgba, texColor1.rgba, 0.5) * vertColor;

}