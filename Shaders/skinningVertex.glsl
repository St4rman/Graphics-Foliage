#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColor;


in vec3 	position;
in vec2 	texCoord;
in vec4 	jointWeights;
in ivec4 	jointIndices;

uniform mat4 joints[128];

out Vertex {
	vec2 texCoord;
	// vec4 colour;
	
}OUT;

void main(void)	{

	vec4 localPos = vec4(position, 1.0f);
	vec4 skelPos  = vec4(0, 0, 0,0);

	for(int i =0; i<4; ++i){
		int jointIndex = jointIndices[i];
		float jointWeights = jointWeights[i];

		skelPos += joints[jointIndex] *localPos * jointWeights;
	}

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;

	gl_Position	  = mvp * vec4(skelPos.xyz, 1.0);
	OUT.texCoord  = texCoord;
	// OUT.colour 	  = nodeColor;
}