#version 330 core
out vec4 frag_color;
out vec2 gsTexCoord;
in vec2 TexCoord;
in vec2 fragPosition;

uniform vec4 vertColor;
uniform sampler2D texSampler;
uniform sampler2D texSampler1;
uniform vec2 cursorPosition;
uniform float cursorRadius;
uniform bool rayCast;

float sdCircle( vec2 p, float r )
{
    return length(p) - r;
}


void main()
{
//        float d = sdCircle(cursorPosition, cursorRadius);
//        if (length(cursorPosition - fragPosition) < cursorRadius && rayCast) {
//            if (length(cursorPosition - fragPosition) < d) {
//                frag_color = vec4(0.0, 1.0, 0.0, 1.0);
//            }
//            else {
//                frag_color = vec4(0.0, 0.0, 1.0, 1.0);
//            }
////            frag_color = vec4(1.0, 0.0, 0.0, 1.0);
//        }
//        else {
            frag_color = texture(texSampler1, gsTexCoord);
//        }


////    vec4 texColor = texture(texSampler, TexCoord);
//    vec4 texColor1 = texture(texSampler1, TexCoord);
//    frag_color = mix(texColor.rgb, texColor1.rgb, texColor1.a);
//    frag_color = mix(texColor.rgba, texColor1.rgba, 0.5) * vertColor;

}