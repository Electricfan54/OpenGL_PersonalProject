#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 _normal;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;



void main()
{
	gl_Position = mvp * position;

	fragPos = vec3(view * model * position);

	uv = TexCoord;
	normal = mat3(normalMat) * _normal;
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

uniform vec4 uLightPos;
uniform vec4 u_Color;
uniform vec4 lightColor;
uniform float ambientLight;
uniform float specularLight;
uniform int specularPow;

// struct Light
// {
// 	vec3 lightPos;
// 	vec4 lightColor;
// }

// struct Material
// {
//     float ambient;
// 	float specular;
// 	int specularPow;
// }

// float LightCalc(Light light, vec3 fragPos, vec3 norm)
// {
// 	vec3 lightDir = normalize(vec3(light.lightPos) - fragPos);

// 	vec4 ambient = light.ambient * light.lightColor;
	
// 	Diffuse
// 	float diffuseStr = max(dot(norm, lightDir), 0.0);
// 	vec4 diffuse = diffuseStr * light.lightColor;

// 	Specular
// 	vec3 viewDir = normalize(-fragPos);
// 	vec3 halfwayDir = normalize(lightDir + viewDir);

// 	float specularAngle = pow(max(dot(norm, halfwayDir), 0.0), specularPow);
// 	vec4 specular = specularAngle * light.lightColor;

// 	Attenuation
// 	float dist = pow(light.lightPos.x - fragPos.x, 2) + pow(light.lightPos.y - fragPos.y, 2) + pow(light.lightPos.z - fragPos.z, 2);
// 	dist = sqrt(dist);
// 	float attenuation = 1.0 / dist;
// }

void main()
{
	vec4 coord = gl_FragCoord;
	//coord = coord * 2 - 1;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(uLightPos) - fragPos);

	vec4 ambient = ambientLight * lightColor;
	
	//Diffuse
	float diffuseStr = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diffuseStr * lightColor;

	//Specular
	vec3 viewDir = normalize(-fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float specularAngle = pow(max(dot(norm, halfwayDir), 0.0), specularPow);
	vec4 specular = specularAngle * lightColor;

	//Attenuation
	float dist = pow(uLightPos.x - fragPos.x, 2) + pow(uLightPos.y - fragPos.y, 2) + pow(uLightPos.z - fragPos.z, 2);
	dist = sqrt(dist);
	float attenuation = 1.0 / dist;
	//attenuation *= .1;

	color = u_Color * (diffuse + specular + ambient) * attenuation;
	color.w = u_Color.w;

	//Gamma Correction
	//float gamma = 2.2;
    //color.rgb =  pow(color.rgb, vec3(1.0/gamma));
}