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
	vec3 nWorldPos;
	vec3 normal;
} IN;

out vec4 fragColour;

void main(void) {

	vec2 uv 		= IN.texCoord;
	vec3 objectPos	= IN.nWorldPos;


	vec4 tipGreen 		= vec4(0.2824, 0.749, 0.2314, 1.0);
	vec4 bottomGreen 	= vec4(0.0, 0.2588, 0.1098, 1.0);

	vec4 bladeCol 	 = uLerp(tipGreen, bottomGreen, uv.y);
	vec4 windValue   =  texture2D(diffuseTex, objectPos.xz);
	windValue 		*= vec4(0.3725, 0.9529, 0.3059, 1.0);

	if(useTexture == 0){

		vec4 finCol = bladeCol;

		// if(windValue.x < 0.3 ){
			finCol = uLerp(windValue, bladeCol, uv.y);
		// }
		
		fragColour = finCol;

	}
	else {
		fragColour =  texture2D(diffuseTex, uv);
	}
}