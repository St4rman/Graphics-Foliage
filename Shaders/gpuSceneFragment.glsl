#version 430 core

uniform sampler2D diffuseTex;
uniform int		  useTexture;


layout (binding  = 2, std430) readonly buffer ssbo1 { 
	vec3 positions[400];
	vec4 color;
};


in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void) {
	// fragColour = color;
	fragColour = texture2D(diffuseTex, IN.texCoord);
}