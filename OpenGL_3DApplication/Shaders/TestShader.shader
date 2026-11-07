#shader vertex
#version 400 core

layout(location = 0) in vec4 position;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * position;
}

#shader fragment
#version 400 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}