#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 _normal;


layout(std430, binding = 3) readonly buffer ssbo1
{
	mat4 instanceModel[];
};


out vec2 uv;
out vec3 normal;
out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 vp;

void main()
{
	gl_Position = vp * instanceModel[gl_InstanceID] * position;

	fragPos = vec3(view * instanceModel[gl_InstanceID] * position);

	uv = TexCoord;

	mat4 normalMat = instanceModel[gl_InstanceID] * view;
	normalMat = inverse(normalMat);
	normalMat = transpose(normalMat);
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

vec4 calcPointLight()
{
	float maxDist = 50.0f;
	float dist = pow(uLightPos.x - fragPos.x, 2) + pow(uLightPos.y - fragPos.y, 2) + pow(uLightPos.z - fragPos.z, 2);
	dist = sqrt(dist);
	if (dist > maxDist)
	{
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	

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
	float attenuation = 1.0 / dist;

	return (diffuse + specular + ambient) * attenuation;
}

vec4 calcDirectionalLight()
{
	vec4 coord = gl_FragCoord;
	//coord = coord * 2 - 1;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(0.0f, 1.0f, 1.0f));

	vec4 ambient = ambientLight * lightColor;
	ambient = vec4(0,0,0,0);
	
	//Diffuse
	float diffuseStr = max(dot(norm, lightDir), 0.0);
	// For reducing the strenght of the directional light
	float diffuseMod = 0.1f;
	vec4 diffuse = diffuseStr * lightColor * diffuseMod;

	//Specular
	vec3 viewDir = normalize(-fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float specularAngle = pow(max(dot(norm, halfwayDir), 0.0), specularPow);
	vec4 specular = specularAngle * lightColor;

	return (diffuse + specular + ambient);
}

void main()
{
	//vec4 coord = gl_FragCoord;
	////coord = coord * 2 - 1;
	//vec3 norm = normalize(normal);
	//vec3 lightDir = normalize(vec3(uLightPos) - fragPos);
	//
	//vec4 ambient = ambientLight * lightColor;
	//
	////Diffuse
	//float diffuseStr = max(dot(norm, lightDir), 0.0);
	//vec4 diffuse = diffuseStr * lightColor;
	//
	////Specular
	//vec3 viewDir = normalize(-fragPos);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//
	//float specularAngle = pow(max(dot(norm, halfwayDir), 0.0), specularPow);
	//vec4 specular = specularAngle * lightColor;
	//
	////Attenuation
	//float dist = pow(uLightPos.x - fragPos.x, 2) + pow(uLightPos.y - fragPos.y, 2) + pow(uLightPos.z - fragPos.z, 2);
	//dist = sqrt(dist);
	//float attenuation = 1.0 / dist;
	////attenuation *= .1;
	//
	//color = u_Color * (diffuse + specular + ambient) * attenuation;

	color = u_Color * (calcPointLight() + calcDirectionalLight());
	color.w = u_Color.w;

	//Gamma Correction
	//float gamma = 2.2;
    //color.rgb =  pow(color.rgb, vec3(1.0/gamma));
}