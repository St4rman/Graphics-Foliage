#version 430 core

uniform sampler2D diffuseTex;
uniform int		  useTexture;
uniform float 	  t;

float contrast   = 0.99;
float saturation = 1.9;
float brightness = 0.1;
float gamma      = 2.2;

vec4 AoColor = vec4( 0,0,0,1);
vec4 topGreen 		= vec4(0.2824, 0.749, 0.2314, 1.0);
vec4 bottomGreen 	= vec4(0.1294, 0.3294, 0.1059, 1.0);
vec4 tip			= vec4(1,1,1,1);

uniform vec3 lightPos;

layout (binding  = 2, std430) readonly buffer ssbo1 { 
	vec3 positions[40000];
	vec4 color;
};


in Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 nWorldPos;
	vec3 normal;
} IN;

out vec4 fragColour;


mat4 satMatrix( float saturation){
	vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 );
    
    float oneMinusSat = 1.0 - saturation;
    
    vec3 red = vec3( luminance.x * oneMinusSat );
    red+= vec3( saturation, 0, 0 );
    
    vec3 green = vec3( luminance.y * oneMinusSat );
    green += vec3( 0, saturation, 0 );
    
    vec3 blue = vec3( luminance.z * oneMinusSat );
    blue += vec3( 0, 0, saturation );
    
    return mat4( red,     0,
                 green,   0,
                 blue,    0,
                 0, 0, 0, 1 );
}

mat4 contrastMatrix(float contrast){
	float t = ( 1.0 - contrast ) / 2.0;
    
    return mat4( contrast, 0, 0, 0,
                 0, contrast, 0, 0,
                 0, 0, contrast, 0,
                 t, t, t, 1 );
}

mat4 brightnessMatrix( float brightness )
{
    return mat4( 1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 brightness, brightness, brightness, 1 );
}


void main(void) {

	vec2 uv 		= IN.texCoord;
	vec3 objectPos	= IN.nWorldPos;

	vec4 bladeCol 	 = mix(topGreen, bottomGreen, uv.y);

	vec4 windValue   =  texture2D(diffuseTex, objectPos.xz);
	// windValue 		*= vec4(1.0, 1.0, 1.0, 1.0);

	vec4 aoCol 		= mix(vec4(1.0f), AoColor, uv.y);
	float tip 		= mix(0.0f, float(tip), uv.y * uv.y);

	vec4 finCol = vec4(0,0,0,1);

	if(useTexture == 0){
		
		finCol = bladeCol;
		finCol *= aoCol;
		finCol = windValue;
	}
	else {
		finCol =  texture2D(diffuseTex, uv);
	}
	
	fragColour = contrastMatrix(contrast) * satMatrix(saturation) *finCol;
	fragColour.rgb = pow(fragColour.rgb, vec3(1.0/gamma));

}