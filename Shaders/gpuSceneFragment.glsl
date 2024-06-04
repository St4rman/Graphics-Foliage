#version 430 core

uniform sampler2D diffuseTex;
uniform int		  useTexture;

layout (binding  = 2, std430) readonly buffer ssbo1 { 
	vec3 positions[16];
};

in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;
void main(void) {
	fragColour = vec4(positions[0], 1.0);
	// fragColour = texture(diffuseTex, IN.texCoord);
	// fragColour = vec4(1.0,0.0,0.0,1.0);
	
}