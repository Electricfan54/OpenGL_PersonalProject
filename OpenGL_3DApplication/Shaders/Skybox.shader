#shader vertex
#version 430 core

layout(location = 0) in vec4 position;

out vec3 texCoords;

uniform mat4 view;
uniform mat4 projection;


void main()
{
	texCoords = vec3(position);
	mat4 modifiedView = mat4(mat3(view));
	vec4 pos = projection * modifiedView * position;
	gl_Position = pos.xyww;
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{
	color = texture(skybox, texCoords);
}