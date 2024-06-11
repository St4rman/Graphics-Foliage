#version 430 core

layout(local_size_x = 10, local_size_y =10, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(location = 0) uniform float t;

uniform vec3 cameraPos;
uniform vec3 mapSize;
uniform vec2 scaley;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec3 voroNoise(vec2 st, float angleOffset){

    st *= 2;
    vec3 color = vec3(.0);

    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 10.;  // minimum distance
    vec2 m_point;        // minimum point

    for (int j=-1; j<=1; j++ ) {
        for (int i=-1; i<=1; i++ ) {
            vec2 neighbor = vec2(float(i),float(j));
            vec2 point = random2(i_st + neighbor);
            point = 0.5 + 0.5*sin(angleOffset* point);
            vec2 diff = neighbor + point - f_st;
            float dist = length(diff);

            if( dist < m_dist ) {
                m_dist = dist;
                m_point = point;
            }
        }
    }

    color += m_dist;
	color = vec3(smoothstep(0.00001,0.7, m_dist));

    return color;
}

layout(binding = 2, std430) buffer ssbo1 {
	vec3 positions[40000];
	vec4 color;
};

//CONDITIONAL BOTH SCALES SHOULD BE EQUAL FOR THIS TO WORK 
int getArrayFromUV(vec2 uv){
	return int( scaley.x *gl_WorkGroupSize.x* uv.x) +   int(uv.y);
}

void calcChunk(vec3 uv){
	
}

void populatePosition(vec2 uv){

	vec3 tempWorldPos;
	tempWorldPos.x =  uv.x *  mapSize.x/float(scaley.x * gl_WorkGroupSize.x );
	tempWorldPos.z =  uv.y *  mapSize.z/float(scaley.y * gl_WorkGroupSize.y);

	tempWorldPos.xz += random2(uv) *mapSize.xz/float(scaley.x * gl_WorkGroupSize.x );
	positions[getArrayFromUV(uv)] = tempWorldPos;	
	
}


void main(){
	
 	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	populatePosition(uv);
	calcChunk(gl_WorkGroupID);

	vec3 col = 0.5 + 0.5*cos(0.+uv.xyx +vec3(0,2,4));
	ivec2 st = ivec2(uv.xy);

	vec3 vnoise = voroNoise(st/100.0, 0.5 * t);
	imageStore(imgOutput, st, vec4(vnoise, 1.0));
}