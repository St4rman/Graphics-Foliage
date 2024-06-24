#version 430 core

uniform mat4 		modelMatrix;
uniform mat4 		viewMatrix;
uniform mat4 		projMatrix;
uniform vec4 		nodeColor;
uniform sampler2D 	diffuseTex;

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
	vec3 nWorldPos;
	
}OUT;


void main(void)	{

	vec3 pos = position;
	// if(pos.y >= 0.1f){
	// 	pos.xz += 0.3* sin(gl_InstanceID) * sin(t);
	// }

	vec3 worldPosition = vec3(positions[gl_InstanceID].x , 0, positions[gl_InstanceID].z) + pos;
	vec3 nWorldPos     = normalize(worldPosition);


	vec4 noiseCol = texture2D(diffuseTex, vec2(0.5,0.5));

	gl_Position	  = (projMatrix * viewMatrix * modelMatrix) * vec4(worldPosition, 1.0);
	OUT.texCoord  = texCoord;
	OUT.colour 	  = noiseCol;
	OUT.nWorldPos = nWorldPos;
}