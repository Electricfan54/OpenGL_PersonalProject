#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 _normal;

out vec2 uv;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * position;

	uv = TexCoord;
	normal = mat3(model) * _normal;
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec2 uv;
in vec3 normal;
in vec3 lightPos;
in vec3 fragPos;

uniform vec4 u_Color;
uniform vec4 lightColor;

void main()
{
	color = u_Color;
	//Gamma Correction
	//float gamma = 2.2;
    //color.rgb =  pow(color.rgb, vec3(1.0/gamma));
}