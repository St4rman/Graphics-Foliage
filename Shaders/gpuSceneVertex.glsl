#version 430 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColor;
uniform vec3 offset[16];

layout(binding = 2, std430) readonly buffer ssbo1 {
	vec3 positions[16];
};

in vec3 position;
in vec2 texCoord;



out Vertex {
	vec2 texCoord;
	vec4 colour;
	
}OUT;

void main(void)	{
	vec3 off = offset[gl_InstanceID];
	mat4 temp = modelMatrix;

	temp[3][0] += positions[gl_InstanceID].x;
	// temp[3][1] += off.y;
	temp[3][2] += positions[gl_InstanceID].z;

	gl_Position	  = (projMatrix * viewMatrix * temp) * vec4(position , 1.0);
	OUT.texCoord  = texCoord;
	OUT.colour 	  = nodeColor;
}