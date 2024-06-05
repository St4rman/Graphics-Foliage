#version 430 core

layout(local_size_x = 1, local_size_y =1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(location = 0) uniform float t;
uniform vec3 cameraPos;
uniform vec3 mapSize;

layout(binding = 2, std430) buffer ssbo1 {
	vec3 positions[400];
	vec4 color;
};

int getArrayFromUV(vec2 uv){
	return int(uv.x) + 20 * int(uv.y);
}


void calcChunk(vec2 uv){
	color = vec4(0,1,0,1);
}


void populatePosition(vec2 uvID){

	vec3 temp;
	temp.x = uvID.x * mapSize.x/20.0f;
	temp.z = uvID.y * mapSize.z/20.0f;
	positions[getArrayFromUV(uvID)] = temp;	

}


void main(){
	
 	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	populatePosition(uv);
	calcChunk(uv);

}