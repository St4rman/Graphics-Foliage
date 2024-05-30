#version 430 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

layout (std140) uniform Matrices{
	float a;
	float b;
};

layout(binding = 2, std430) readonly buffer ssbo1 {
	vec4 temp;
};

in vec3 position;
in vec4 colour; 
in vec2 texCoord;

out Vertex {
	vec4 colour;
	vec2 texCoord;
}OUT;

void main(void)	{
	// gl_Position	  = vec4(position, 1.0);
	OUT.colour 	  = colour;
	OUT.texCoord = texCoord;
	gl_Position = vec4(position, 1.0);
}