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
uniform vec4 color_diffuse = vec4(0.3f, 0.3f, 0.3f, 1.0f);
uniform vec4 color_specular = vec4(0.7f, 0.7f, 0.7f, 1.0f);
uniform float const_diffuse = 2.0f;
uniform float exponent_specular = 5.0f;
uniform float const_specular = 5.0f;

uniform vec3 color_fog = vec3(0.5f, 0.5f, 0.8f);

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normalmap;
uniform sampler2D tex_shadowmap;
uniform bool useTexture;
uniform bool useNormalmap = false;

vec3 CalcNewNormal(vec3 fs_normal) {
	vec3 tex_normal = texture(texture_normalmap, vs_uv).xyz;
	mat3 rotTangent = mat3(1, 0, 0, 0, 0, -1, 0, 1, 0);
	vec3 tangent = rotTangent * fs_normal;
	tangent = normalize(tangent - dot(tangent, fs_normal) * fs_normal);
	vec3 biTangent = cross(tangent, fs_normal);
	vec3 bumpMapNormal = tex_normal.xyz;
	bumpMapNormal = 2.0 * bumpMapNormal - vec3(1,1,1);
	vec3 newNormal;
	mat3 TBN = mat3(tangent, biTangent, fs_normal);
	newNormal = normalize(TBN * bumpMapNormal);
	return newNormal;
}

void main() {

	vec3 lightToPos_vector[MAX_NUM_LIGHT];
	float lightToPos_distance[MAX_NUM_LIGHT];
	vec3 lightReflect_vector[MAX_NUM_LIGHT];
	float diffuse_factor = 0.0f;
	float specular_factor = 0.0f;
	vec3 fs_normal = normalize(vs_normal);
	vec3 temp_vec3;
	
	if(useNormalmap == true){
		fs_normal = CalcNewNormal(fs_normal);
	}

	// Sunlight
	lightReflect_vector[0] = reflect(light_direction[0], fs_normal);
	diffuse_factor += max(dot(-light_direction[0], fs_normal), 0.0f) * light_intensity[0];
	specular_factor += max(pow(dot(lightReflect_vector[0], normalize(posToEye_vector)), exponent_specular), 0.0f) * light_intensity[0];

	// Pointlight
	for(int i = 1; i < numLight; i++) {
		temp_vec3 = vs_position - light_position[i];
		lightToPos_vector[i] = normalize(temp_vec3);
		lightToPos_distance[i] = (temp_vec3 / lightToPos_vector[i]).x;
		lightReflect_vector[i] = reflect(lightToPos_vector[i], fs_normal);
		diffuse_factor += max(dot(-lightToPos_vector[i], fs_normal), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
		specular_factor += max(pow(dot(lightReflect_vector[i], normalize(posToEye_vector)), exponent_specular), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
	}
	finalColor = color_ambient + diffuse_factor * color_diffuse * const_diffuse + specular_factor * color_specular * const_specular;
	finalColor.a = 1;

	if(useTexture == true) {
		vec4 texture_color = texture(texture_diffuse, vs_uv);
		finalColor *= texture_color;
	}
	finalColor = clamp(finalColor, 0, 1);
	//finalColor = mix(vec4(color_fog, 1), finalColor, visibility);
	//finalColor = texture(texture_normalmap, vs_uv);

	//float bias = 0.005f * tan(acos(diffuse_factor));
	//bias = clamp(bias, 0, 0.001f);
	//if(vs_shadow.z - bias > texture(tex_shadowmap, vs_shadow.xy).x) {
	//	finalColor = mix(vec4(0, 0, 0, 1), finalColor, 0.3f);
	//}
}