#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;

uniform sampler2D texSampler1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//frag_color = vertColor;
	vec4 texCol = texture(texSampler1, TexCoord);

	float K_ambient = 0.2;
	float K_diffuse = 0.5;
	float K_specular = 1;
	float n_shiny = 50;
	float intensity = 20000;
	
	vec3 V = normalize(viewPos - FragPos);

	vec3 I_ambient = K_ambient * lightColor;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float NdotL = dot(normal, lightDir);
	vec3 I_diffuse = K_diffuse * NdotL * lightColor;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float RdotV = max(dot(reflectDir, viewDir), 0.0f);
	vec3 I_specular = K_specular * pow(RdotV, n_shiny) * lightColor;


	frag_color = vec4(I_ambient + I_diffuse + I_specular, 1.0f) * texCol;
}