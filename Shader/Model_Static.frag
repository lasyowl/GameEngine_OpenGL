#version 430 core

#define MAX_NUM_LIGHT 100

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec2 vs_uv;
layout(location = 4) in vec3 posToEye_vector;
layout(location = 6) in float visibility;

layout(location = 0) out vec4 finalColor;

uniform int numLight = 0;
uniform float light_intensity[MAX_NUM_LIGHT];
uniform vec3 light_position[MAX_NUM_LIGHT];
uniform vec3 light_direction[MAX_NUM_LIGHT];

uniform float coef_ambient = 0.3f;
uniform vec4 color_ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
uniform vec4 color_diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
uniform vec4 color_specular = vec4(0.7f, 0.7f, 0.7f, 1.0f);
uniform float exponent_specular = 5.0f;

uniform vec3 color_fog = vec3(0.5f, 0.5f, 0.8f);

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normalmap;
uniform bool useTexture;
uniform bool useNormalmap;
uniform bool reflectMode;

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
	if(useNormalmap == true) {
		fs_normal = CalcNewNormal(fs_normal);
	}

	for(int i = 0; i < numLight; i++) {
		lightToPos_vector[i] = normalize(vs_position - light_position[i]);
		lightToPos_distance[i] = distance(vs_position, light_position[i]);
		lightReflect_vector[i] = reflect(lightToPos_vector[i], fs_normal);
		diffuse_factor += max(dot(-lightToPos_vector[i], fs_normal), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
		specular_factor += pow(max(0.0f, dot(lightReflect_vector[i], normalize(posToEye_vector))), exponent_specular) * light_intensity[i] / pow(lightToPos_distance[i], 2);
	}

	if(useTexture == true) {
		vec4 texture_color = texture(texture_diffuse, vs_uv);
		finalColor = coef_ambient * texture_color + texture_color * (diffuse_factor * color_diffuse) + specular_factor * color_specular;
		finalColor.w = texture_color.w;
	}
	else {
		finalColor = color_ambient + diffuse_factor * color_diffuse + specular_factor * color_specular;
		finalColor.w = color_diffuse.w;
	}

	if(finalColor.a < 0.1f)
		discard;

	//finalColor = mix(vec4(color_fog, 1), finalColor, visibility);
}