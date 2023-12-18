#version 330 core
in vec3 norm;
in vec3 fragPos;
in vec2 TexCoords;

out vec4 FragColor;
uniform vec3 viewPos;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D emission;
};

/*struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};*/

struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
	vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS]; 

struct SpotLight{
	vec3 position;
	vec3 spotDir;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;


uniform Material material;
// uniform Light light;
uniform float deltaY;
uniform float MatrixLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	vec3 norm = normalize(normal);
	vec3 diffuse = light.diffuse * max(dot(lightDir, norm), 0.0) * vec3(texture(material.diffuse, TexCoords));
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * vec3(texture(material.specular, TexCoords));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	return ambient + diffuse + spec;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(pointLight.position - fragPos);
	vec3 norm = normalize(normal);
	vec3 reflectDir = reflect(-lightDir, norm);
	float distance = length(fragPos - pointLight.position);

	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = pointLight.diffuse * max(dot(lightDir, norm), 0.0) * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = pointLight.specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * texture(material.specular, TexCoords).rgb;
	
	float attenuation = 1.0 / (pointLight.constant, pointLight.linear * distance + pointLight.quadratic * distance * distance);
	
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(spotLight.position - fragPos);
	vec3 norm = normalize(normal);
	vec3 reflectDir = reflect(lightDir, norm);
	float theta = dot(-lightDir, normalize(spotLight.spotDir));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = spotLight.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 diffuse = spotLight.diffuse * max(dot(lightDir, norm), 0.0) * texture(material.diffuse, TexCoords).rgb;
	vec3 specular = spotLight.specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * texture(material.specular, TexCoords).rgb;

	return ambient + (diffuse + specular) * intensity;
}


void main(){
	/*vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(light.position - fragPos);
	// vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoords).rgb * spec * light.specular;

	float theta = dot(-lightDir, normalize(light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);
	
	vec3 result = ambient + (diffuse + specular) * intensity;

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// float x = TexCoords.x;
	// float y = TexCoords.y;
	// if(x >= 0.1 && x <= 0.9 && y >= 0.1 && y <= 0.9)
	//		result = result + MatrixLight * texture(material.emission, vec2(TexCoords.x + deltaY, TexCoords.y + deltaY)).rbg;
	FragColor = vec4(result, 1.0);*/

	vec3 normal = normalize(norm);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	
	for(int i = 0; i < NR_POINT_LIGHTS; ++i){
		result += CalcPointLight(pointLights[i], normal, fragPos, viewDir);
	}
	
	result += CalcSpotLight(spotLight, normal, fragPos, viewDir);

	FragColor = vec4(result, 1.0);
}