#version 330 core 
in vec3 normal;
in vec3 fragPos;
in vec3 texCoord;
out vec4 FragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D tDiffuse; //textures
	sampler2D tSpecular; //specular textures
	float shininess; //radius of specular highlighting.
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

vec3 CalcDirLight(DirLight light, vec3 _normal, vec3 viewDir, Material mat);
vec3 CalcPointLight(PointLight light, vec3 _normal, vec3 frag_pos, vec3 viewDir, Material mat);

#define NB_POINT_LIGHTS 100
#define NB_MATERIALS 5
uniform Material material[NB_MATERIALS];
uniform PointLight l_points[NB_POINT_LIGHTS];



uniform DirLight l_directional;
uniform vec3 viewPos;


uniform int render_mode;

void main()
{
	if (render_mode == 2) {
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	} else {
		vec3 norm = normalize(normal);
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 result = CalcDirLight(l_directional, norm, viewDir, material[int(texCoord.z)]);
		for(int i = 0; i < NB_POINT_LIGHTS; i++)
			if (l_points[i].constant > 0) {
				result += CalcPointLight(l_points[i], norm, fragPos, viewDir, material[int(texCoord.z)]);  
			}
	
		FragColor = vec4(result, 1.0);
	}


}

vec3 CalcDirLight(DirLight light, vec3 _normal, vec3 viewDir, Material mat) {
	vec3  l_dir = normalize(-light.direction);
	float diffuse = max(dot(_normal, l_dir), 0.0);
	vec3 reflectDir = reflect(-l_dir, _normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	
	vec3 ambient = mat.ambient * light.ambient * vec3(texture(mat.tDiffuse, texCoord.xy));
	vec3 diffusion = mat.diffuse * light.diffuse * diffuse * vec3(texture(mat.tDiffuse, texCoord.xy));
	vec3 specular = mat.specular * light.specular * spec * vec3(texture(mat.tSpecular, texCoord.xy));
	return (ambient + diffusion);
}

vec3 CalcPointLight(PointLight light, vec3 _normal, vec3 frag_pos, vec3 viewDir, Material mat) {
	vec3 l_dir = normalize(light.position - frag_pos);
	float diff = max(dot(_normal, l_dir), 0.0);
	
	vec3 reflectDir = reflect(-l_dir, _normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	float dist = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient = mat.ambient * light.ambient * vec3(texture(mat.tDiffuse, texCoord.xy));
	vec3 diffusion = mat.diffuse * light.diffuse * diff * vec3(texture(mat.tDiffuse, texCoord.xy));
	vec3 specular = mat.specular * light.specular * spec * vec3(texture(mat.tSpecular, texCoord.xy));
	ambient *= attenuation;
	diffusion *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffusion);
}