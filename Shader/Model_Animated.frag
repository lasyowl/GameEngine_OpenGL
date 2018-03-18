#version 430 core

#define MAX_NUM_LIGHT 100

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec2 vs_uv;
layout(location = 4) in vec3 posToEye_vector;
layout(location = 6) in float visibility;
layout(location = 14) in vec3 vs_shadow;

layout(location = 0) out vec4 finalColor;

uniform int numLight = 0;
uniform float light_intensity[MAX_NUM_LIGHT];
uniform vec3 light_position[MAX_NUM_LIGHT];
uniform vec3 light_direction[MAX_NUM_LIGHT];

uniform vec4 color_ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
uniform vec4 color_diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
uniform vec4 color_specular = vec4(0.7f, 0.7f, 0.7f, 1.0f);
uniform float exponent_specular = 5.0f;

uniform vec3 color_fog = vec3(0.5f, 0.5f, 0.8f);

uniform sampler2D texture_diffuse;
uniform sampler2D tex_shadowmap;
uniform bool useTexture;

void main(){

	vec3 lightToPos_vector[MAX_NUM_LIGHT];
	float lightToPos_distance[MAX_NUM_LIGHT];
	vec3 lightReflect_vector[MAX_NUM_LIGHT];
	float diffuse_factor = 0.0f;
	float specular_factor = 0.0f;
	vec3 fs_normal = normalize(vs_normal);

	for(int i = 0; i < numLight; i++) {
		lightToPos_vector[i] = normalize(vs_position - light_position[i]);
		lightToPos_distance[i] = distance(vs_position, light_position[i]);
		lightReflect_vector[i] = reflect(lightToPos_vector[i], fs_normal);
		diffuse_factor += max(dot(-lightToPos_vector[i], fs_normal), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
		specular_factor += pow(max(0.0f, dot(lightReflect_vector[i], normalize(posToEye_vector))), exponent_specular) * light_intensity[i] / pow(lightToPos_distance[i], 2);
	}
	finalColor = color_ambient + diffuse_factor * color_diffuse + specular_factor * color_specular;

	if(useTexture == true) {
		vec4 texture_color = texture(texture_diffuse, vs_uv);
		finalColor *= texture_color;
	}
	finalColor = clamp(finalColor, 0, 1);
	finalColor = mix(vec4(color_fog, 1), finalColor, visibility);

	float bias = 0.005f * tan(acos(diffuse_factor));
	//bias = clamp(bias, 0, 0.001f);
	//if(vs_shadow.z - bias > texture(tex_shadowmap, vs_shadow.xy).x)
		//finalColor = mix(vec4(0, 0, 0, 1), finalColor, 0.3f);
}