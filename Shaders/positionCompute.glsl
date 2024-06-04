#version 430 core

layout(local_size_x = 1, local_size_y =1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(location = 0) uniform float t;

layout (std140) uniform data{
	float a;
};

layout(binding = 2, std430) buffer ssbo1 {
	vec3 positions[16];
};

int getArrayFromUV(vec2 uv){
	return 4* int(uv.x) + int(uv.y);
}

void populatePosition(vec2 uvID){
	vec3 temp;
	temp.x = uvID.x * 300.0f;
	temp.z = uvID.y * 300.0f;
	positions[getArrayFromUV(uvID)] = temp;
}

void main(){

 	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	vec4 col = vec4(0,0,0,1);
	populatePosition(uv);

	col.x = float(uv.x)  / (gl_NumWorkGroups.x);
	col.y = (float (uv.y)/gl_NumWorkGroups.y);

	imageStore(imgOutput, uv, col);

}