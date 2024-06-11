#version 430 core

uniform sampler2D diffuseTex;
uniform int		  useTexture;
uniform float 	  t;

uniform vec3 lightPos;

layout (binding  = 2, std430) readonly buffer ssbo1 { 
	vec3 positions[40000];
	vec4 color;
};

vec4 uLerp(vec4 a, vec4 b, float t){
	return mix(a, b, t);
}

//returns a positive dot product
float posDot (vec3 v1, vec3 v2){
	return max(0.0f, dot(v1, v2));
}

in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour;

void main(void) {

	vec2 uv = IN.texCoord;

	vec4 tipGreen 		= vec4(0.678,0.898,0.655,1);
	vec4 bottomGreen 	= vec4(0.302,0.451,0.365,1);

	vec4 bladeCol 	 = uLerp(tipGreen, bottomGreen, uv.y);

	if(useTexture == 0){
		// fragColour = bladeCol;
		fragColour = bladeCol * diffuseTex.r;
	}
	else {
		fragColour = bladeCol; 
	}
}