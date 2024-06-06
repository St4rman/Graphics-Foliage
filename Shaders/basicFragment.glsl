#version 430 core
uniform sampler2D diffuseTex;

in Vertex{
	vec4 colour;
	vec2 texCoord;
} IN;

layout (std140) uniform Matrices{
	float a;
	float b;
};

layout (binding  = 2, std430) readonly buffer ssbo1 { 
	vec4 temp[2];
};

out vec4 fragColour;
void main(void) {
	vec2 uv 	= IN.texCoord;
	vec4 col 	= IN.colour;
	vec3 cola 	= vec3(0.0f);
	fragColour 	= texture2D(diffuseTex, uv);
}