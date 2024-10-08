#version 330 core

uniform sampler2D diffuseTex;

in Vertex{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColour;
void main(void) {
	fragColour = texture(diffuseTex, IN.texCoord);
	// fragColour *= vec4(1,1,1,1);
	// fragColour = vec4(1,0,0,1);
}