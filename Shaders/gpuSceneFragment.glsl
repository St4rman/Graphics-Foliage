#version 430 core

uniform sampler2D diffuseTex;
uniform int		  useTexture;


layout (binding  = 2, std430) readonly buffer ssbo1 { 
	vec3 positions[2500];
	vec4 color;
};

vec4 uLerp(vec4 a, vec4 b, float t){
	return mix(a, b, t);
}

in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;

void main(void) {

	vec2 uv = IN.texCoord;

	vec4 tipGreen = vec4(0.0, 0.8706, 0.3216, 1.0);
	vec4 bottomGreen =vec4(0.6392, 1.0, 0.3804, 1.0);
	vec4 col = uLerp(tipGreen, bottomGreen, uv.y);

	if(useTexture == 0){
		fragColour = col;
	}
	else {
		fragColour = texture2D(diffuseTex, IN.texCoord);
	}
}