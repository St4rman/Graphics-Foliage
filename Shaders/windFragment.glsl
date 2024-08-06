#version 330 core

uniform sampler2D diffuseTex;
uniform int		  useTexture;

in Vertex{

	vec2 texCoord;
	vec4 colour;
	
} IN;

out vec4 fragColour;
void main(void) {

	vec2 uv    = IN.texCoord;
	fragColour = IN.colour;
	fragColour = vec4(uv, 0, 1);
	
}