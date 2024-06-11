#version 430 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColor;
uniform sampler2D diffuseTex;

uniform float t;
layout(location = 1) uniform vec3 cameraPos;

layout(binding = 2, std430) readonly buffer ssbo1 {
	vec3 positions[40000];
	vec4 color;
};

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
	vec4 colour;
	
}OUT;


void main(void)	{

	mat4 temp = modelMatrix;

	temp[3][0] += positions[gl_InstanceID].x;
	temp[3][2] += positions[gl_InstanceID].z;

	vec3 pos = position;

	if(pos.y >= 0.1f){
		pos.xz += 0.3* sin(gl_InstanceID) * sin(t);
	}

	gl_Position	  = (projMatrix * viewMatrix * temp) * vec4(pos, 1.0);
	OUT.texCoord  = texCoord;
	OUT.colour 	  = nodeColor;
}