#version 430 core

layout(local_size_x = 10, local_size_y =10, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(location = 0) uniform float t;

layout (std140) uniform Matrices{
	float a;
	float b;
};


layout(binding = 2, std430) buffer ssbo1 {
	vec4 temp;
};



void main(){
 	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	vec4 col = vec4(0,0,0,1);

	temp = vec4(0.0f, 1.0f,0.0f,1.0f);

	float speed = 100;
	float width = 1000;
	col.x = float(uv.x)  / (gl_NumWorkGroups.x);
	col.y = (float (uv.y)/gl_NumWorkGroups.y);
	imageStore(imgOutput, uv, col);
}