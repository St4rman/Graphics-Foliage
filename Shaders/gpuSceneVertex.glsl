#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColor;

uniform vec3 offset[16];
in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
	vec4 colour;
	
}OUT;

void main(void)	{
	vec3 off = offset[gl_InstanceID];
	mat4 temp = modelMatrix;

	temp[3][0] += off.x;
	// temp[3][1] += off.y;
	temp[3][2] += off.z;

	gl_Position	  = (projMatrix * viewMatrix * temp) * vec4(position, 1.0);
	OUT.texCoord  = texCoord;
	OUT.colour 	  = nodeColor;
}