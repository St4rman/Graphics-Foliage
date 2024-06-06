#version 430 core

layout(local_size_x = 1, local_size_y =1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(location = 0) uniform float t;
uniform vec3 cameraPos;
uniform vec3 mapSize;
uniform int scale;

layout(binding = 2, std430) buffer ssbo1 {
	vec3 positions[2500];
	vec4 color;
};

int getArrayFromUV(vec2 uv){
	return int(uv.x) + scale * int(uv.y);
}


void calcChunk(vec2 uv){
	color = vec4(0,1,0,1);
}

void populatePosition(vec2 uvID){

	vec2 temp;
	temp.x = uvID.x * mapSize.x/float(scale);
	temp.y = uvID.y * mapSize.z/float(scale);
	
	positions[getArrayFromUV(uvID)] = vec3(temp.x, 0, temp.y);	

}

void main(){
	
 	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	populatePosition(uv);
	calcChunk(uv);

}