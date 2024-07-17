#version 330 core

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void)	{

	vec3 incident = normalize(lightPos- IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	vec4  diffuse = texture(diffuseTex, IN.texCoord);

	float lambert = max(dot(incident, IN.normal), 0.0f);
	float distance = length(lightPos- IN.worldPos);
	float atten = 1.0- clamp(distance / lightRadius, 0.0, 1.0);

	float specFactor =  clamp(dot(halfDir, IN.normal),0.0,1.0);
	specFactor =  pow(specFactor, 60.0 );

	vec3 surface =  (diffuse.rgb * lightColor.rgb);
	fragColour.rgb  = surface* lambert * atten;
	fragColour.rgb += (lightColor.rgb * specFactor)*atten*0.33;
	fragColour.rgb +=  surface * 0.1f;
	fragColour.rgb *= vec3(vec3(0.1098, 0.7882, 0.4039));
	fragColour.a  = diffuse.a;
	
}