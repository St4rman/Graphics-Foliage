#version 430 core

const vec3 UP 		= vec3(0,1,0);
const vec3 RIGHT 	= vec3(1,0,0);
const vec3 FWD 		= vec3(0,0,1);
const float RADIUS  = 1000.0f;
const float LANE_SIZE = 30.0f;

uniform mat4 		modelMatrix;
uniform mat4 		viewMatrix;
uniform mat4 		projMatrix;
uniform vec4 		nodeColor;

uniform int		  	useTexture;
uniform sampler2D 	diffuseTex;
uniform vec3		spacePerBlade;

uniform vec2 		windDir;

uniform float windFwdSway;
uniform float windRightSway;

uniform float time;
uniform vec3 cameraPos;
uniform vec3 grassDims;

layout(binding = 2, std430) readonly buffer ssbo1 {
	vec3 positions[2560000];
	vec4 grassDimensions;
};

layout(binding = 3, std430) buffer heightBuffer{
    float yPos[2560000];
};

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec3 normal;

out Vertex {
	vec2 texCoord;
	vec3 cameraReg;
	float dist;
	vec3 nWorldPos;
	vec3 normal;
}OUT;

mat4 scale(vec3 _scale){
	return mat4(_scale.x, 0, 0, 0,
				0, _scale.y ,0,0,
				0, 0, _scale.z, 0,
				0, 0, 0, 1);
}

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

//CameraRegion
float camRegion(vec3 camNorm, vec3 wPos){

	float h = camNorm.x - abs(wPos.x);
	float w = camNorm.z - abs(wPos.z);

	float radialDist = pow(w, 2.0)  + pow(h, 2.0);
	return radialDist / pow(RADIUS, 2.0);
}


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

float heightBasedBladeLen(){
	
	float tempY = (yPos[gl_InstanceID] /grassDimensions.y);
	return smoothstep(1.0f, 5.0f, tempY);

}

void main(void)	{

	vec3 mGrassDims = grassDims * vec3(1, heightBasedBladeLen(), 1); 
	mat4 scaledModel = scale(mGrassDims);

	vec3 pos 		= position;
	vec3 camNorm    = abs(cameraPos) / mGrassDims;

	vec3 worldPosCache = vec3(positions[gl_InstanceID].x , 0, positions[gl_InstanceID].z);
	float windStrength = texture2D(diffuseTex,(worldPosCache/spacePerBlade).xz).x;

	if(pos.y > 0.1f && useTexture == 0){

		vec3 windFwd = mat3(scaledModel) * vec3(windDir.x, 0, -windDir.y);
		vec3 windRight = normalize(cross(windFwd, UP));

		float x =  radians(windRightSway) * sin(time)* windStrength;
		float z = radians(windFwdSway)  * windStrength;
		
		//flipping this here because for some reasion these are flipped
		pos = rotMat(z, windRight) *rotMat(x, windFwd) * pos;
		pos.y += heightBasedBladeLen() * random2(worldPosCache.xz).x;
	}
	
	//this is the final postion. Any changes to individual blades should be done above
	vec3 worldPosition = vec3(positions[gl_InstanceID].x , (yPos[gl_InstanceID] /mGrassDims.y) +1.9f, positions[gl_InstanceID].z) + pos;
	worldPosition.x *= -1.0f;

	

	gl_Position	  		= (projMatrix * viewMatrix * scaledModel) * vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0);
	OUT.texCoord  		= texCoord;
	OUT.nWorldPos 		= worldPosition/ spacePerBlade;
	OUT.cameraReg		= camNorm - abs(worldPosition);
	OUT.dist     		= camRegion(camNorm, worldPosition);
}