#version 430 core

#define MAX_NUM_LIGHT 100

layout(location = 0) in vec3 gs_position;
layout(location = 1) in float gs_timeRemain;
layout(location = 2) in vec2 gs_texCoord0;
layout(location = 3) in vec2 gs_texCoord1;
layout(location = 4) in vec3 gs_posToEye_vector;
layout(location = 5) in float blendFactor;

layout(location = 0) out vec4 finalColor;

uniform int numLight = 0;
uniform float light_intensity[MAX_NUM_LIGHT];
uniform vec3 light_position[MAX_NUM_LIGHT];
uniform vec3 light_direction[MAX_NUM_LIGHT];

uniform vec3 color_ambient = vec3(0.1f, 0.1f, 0.1f);
uniform vec3 color_diffuse = vec3(0.2f, 0.2f, 0.2f);
uniform vec3 color_specular = vec3(0.7f, 0.7f, 0.7f);
uniform float exponent_specular = 5.0f;

uniform sampler2DArray texture_array;

void main(){
	
	vec3 lightToPos_vector[MAX_NUM_LIGHT];
	float lightToPos_distance[MAX_NUM_LIGHT];
	vec3 lightReflect_vector[MAX_NUM_LIGHT];
	float diffuse_factor = 0.0f;
	float specular_factor = 0.0f;
	//vec3 fs_normal = normalize(vs_normal);

	for(int i = 0; i < numLight; i++) {
		//lightToPos_vector[i] = normalize(vs_position - light_position[i]);
		//lightToPos_distance[i] = distance(vs_position, light_position[i]);
		//lightReflect_vector[i] = reflect(lightToPos_vector[i], fs_normal);
		//diffuse_factor += max(dot(-lightToPos_vector[i], fs_normal), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
		//specular_factor += max(pow(dot(lightReflect_vector[i], normalize(posToEye_vector)), exponent_specular), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
	}

	//finalColor = vec4(color_ambient + diffuse_factor * color_diffuse + specular_factor * color_specular, 1);

	vec4 textureColor = texture(texture_array, vec3(gs_texCoord0, 0.0f)) * (1 - blendFactor) + texture(texture_array, vec3(gs_texCoord1, 0.0f)) * blendFactor;
	//if(texture(texture_array, vec3(gs_texCoord0, 0.0f)).b < 0.01f)
		//discard;
	finalColor = textureColor;
	finalColor.a *= -gs_position.z + 0.5f;
	//finalColor = vec4(1,1,1,1);
}