#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 fragPos;

struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct Material{
	vec3 ambient;
	sampler2D texture_diffuse;
	sampler2D texture_specualr;
	float shininess;
	sampler2D emission;
};
uniform Material material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	vec3 norm = normalize(normal);
	vec3 diffuse = light.diffuse * max(dot(lightDir, norm), 0.0) * vec3(texture(material.texture_diffuse, TexCoords));
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * vec3(texture(material.texture_specualr, TexCoords));

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));

	return ambient + diffuse + spec;
}

void main()
{    
	vec3 result = CalcDirLight(dirLight, Normal, normalize(viewPos - fragPos));
    FragColor = vec4(result, 1.0f);
}