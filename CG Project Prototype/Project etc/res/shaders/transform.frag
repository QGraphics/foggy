#version 330 core

struct Material
{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
	vec3 diffuse;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 frag_color;
out vec2 gsTexCoord;

uniform DirectionalLight dirLight;
uniform Material material;
uniform vec3 viewPos;
uniform sampler2D texSampler;
uniform vec2 cursorPosition;
uniform float cursorRadius;
uniform bool rayCast;

float sdCircle(vec2 p, float r)
{
	return length(p) - r;
}


void main()
{
	vec4 texCol = texture(material.diffuseMap, TexCoord);

	vec3 V = normalize(viewPos - FragPos);

	// Ambient
	vec3 I_ambient = dirLight.ambient * material.ambient;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(-dirLight.direction);
	float NdotL = max(dot(normal, lightDir), 0.0f);
	vec3 I_diffuse = dirLight.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
	
	// Shiny
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float NdotH = max(dot(normal, halfDir), 0.0f);
	vec3 I_specular = material.specular * pow(NdotH, material.shininess) * dirLight.specular;

	vec4 texel = texture(texSampler, TexCoord);

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
			frag_color = vec4(I_ambient + I_diffuse + I_specular, 1.0f) * texel;
//        }


////    vec4 texColor = texture(texSampler, TexCoord);
//    vec4 texColor1 = texture(texSampler1, TexCoord);
//    frag_color = mix(texColor.rgb, texColor1.rgb, texColor1.a);
//    frag_color = mix(texColor.rgba, texColor1.rgba, 0.5) * vertColor;

}