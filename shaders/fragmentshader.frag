#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;  
in vec3 Normal;

struct Light {
    vec3 position;
	vec3 color;
    
    float constant;
    float linear;
    float quadratic;
	
    float ambient;
    float diffuse;
    float specular;
};
#define N_LIGHTS 3
uniform Light lights[N_LIGHTS];

uniform sampler2D texture;
uniform vec3 viewPos;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   
    vec3 ambient = light.ambient * light.color;
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0, 0.0, 0.0);
	for(int i = 0; i < N_LIGHTS; i++)
		result += CalcPointLight(lights[i], Normal, FragPos, viewDir);
	FragColor = texture(texture, TexCoord) * vec4(result, 1.0);
}