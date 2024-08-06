#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColor;

vec3 windScale = vec3(100,100,100);

in vec3 position;
in vec2 texCoord;

out Vertex {
	
	vec2 texCoord;
	vec4 colour;
	
}OUT;

mat4 scale(vec3 _scale){
	return mat4(_scale.x, 0, 0, 0,
				0.0, _scale.y ,0,0,
				0, 0, 0, _scale.z,
				0, 0, 0, 1);
}

//create a line of quads
vec3 offset(){

	if(gl_InstanceID==0){
		int vid = gl_VertexID;
		return vec3(gl_VertexID, 0, 0);
	}
	else return vec3(gl_VertexID, 2, 0);
}

void main(void)	{

	// vec3 offset   = offSets[gl_InstanceID];
	mat4 mMatrix = scale(windScale) * modelMatrix;

	gl_Position	  = (projMatrix * viewMatrix * mMatrix) * vec4(position + offset(), 1.0);
	OUT.texCoord  = texCoord;
	OUT.colour 	  = nodeColor;
}