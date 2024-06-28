#version 430 core

const vec3 UP 		= vec3(0,1,0);
const vec3 RIGHT 	= vec3(1,0,0);
const vec3 FWD 		= vec3(0,0,1);

uniform mat4 		modelMatrix;
uniform mat4 		viewMatrix;
uniform mat4 		projMatrix;
uniform vec4 		nodeColor;
uniform int		  	useTexture;
uniform sampler2D 	diffuseTex;
uniform vec3		mapSize;
uniform vec2 		windDir;

uniform float windFwdSway;
uniform float windRightSway;

uniform float t;
layout(location = 1) uniform vec3 cameraPos;

layout(binding = 2, std430) readonly buffer ssbo1 {
	vec3 positions[40000];
	vec4 color;
};

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec3 normal;

out Vertex {
	vec2 texCoord;
	vec3 worldPos;
	vec3 nWorldPos;
	vec3 normal;
}OUT;

//mutliply anything with this mat4 to rotate it to whatever 
mat3 rotMat(float angle, vec3 axis){
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;
	return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,   oc * axis.z * axis.x + axis.y * s,  
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,            oc * axis.y * axis.z - axis.x * s, 
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,   oc * axis.z * axis.z + c);
	
}


void main(void)	{
	vec3 pos = position;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));

	vec3 worldPosCache = vec3(positions[gl_InstanceID].x , 0, positions[gl_InstanceID].z) + pos;
	float windStrength = texture2D(diffuseTex,(worldPosCache/mapSize).xz).x;
		

	if(pos.y > 0.1f){

		vec3 windFwd = mat3(modelMatrix) * vec3(windDir.x, 0, -windDir.y);
		vec3 windRight = normalize(cross(windFwd, UP));

		float x =  radians(windRightSway) * windStrength;
		float z = radians(windFwdSway) * windStrength;
		//flipping this here because for some reasion these are flipped
		pos = rotMat(z, windRight) *rotMat(x, windFwd) * pos;
	

	}
	
	//this is the final postion. Any changes to individual blades should be done above
	vec3 worldPosition = vec3(positions[gl_InstanceID].x , 0, positions[gl_InstanceID].z) + pos;
	vec3 wPos = worldPosition;
	if(useTexture == 0){
		worldPosition 	  -= mapSize/2;
	}
	
	gl_Position	  		= (projMatrix * viewMatrix * modelMatrix) * vec4(worldPosition, 1.0);
	OUT.texCoord  		= texCoord;
	OUT.nWorldPos 		= wPos/ mapSize;
}