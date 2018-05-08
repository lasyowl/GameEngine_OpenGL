#version 430 core

#define MAX_NUM_LIGHT 100
#define NUM_TERRAINPACK 4

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec2 vs_uv;
layout(location = 3) in vec3 vs_tangent;
layout(location = 4) in vec3 posToEye_vector;
layout(location = 6) in float visibility;
layout(location = 12) in float slope;
layout(location = 13) in float height;
layout(location = 14) in vec3 vs_shadow;

layout(location = 0) out vec4 finalColor;

uniform int numLight = 0;
uniform float light_intensity[MAX_NUM_LIGHT];
uniform vec3 light_position[MAX_NUM_LIGHT];
uniform vec3 light_direction[MAX_NUM_LIGHT];

uniform float coef_ambient = 0.3f;
uniform vec3 color_diffuse = vec3(0.5f, 0.5f, 0.5f);
vec4 color_specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
uniform float const_specular = 5.0f;
uniform float exponent_specular = 5.0f;

uniform vec3 color_fog = vec3(0.5f, 0.5f, 0.8f);

uniform sampler2DArray tex_terrainPack;
uniform sampler2DArray tex_normalPack;
uniform sampler2DArray tex_specularPack;
uniform sampler2D tex_alphamap;
uniform sampler2D tex_shadowmap;
uniform float terrainDensity = 20.0f;

vec3 tex_normal;
vec2 fs_uv;

uniform int highlight;
uniform bool fastMode;

vec4 GetTextureColor() {
	vec4 returnColor;
	vec4 alphaTex = texture(tex_alphamap, vs_uv);
	fs_uv = vs_uv * terrainDensity;

	vec4 terrainTex[NUM_TERRAINPACK];
	vec3 normalTex[NUM_TERRAINPACK];
	vec4 specularTex[NUM_TERRAINPACK];

	for(int i = 0; i < NUM_TERRAINPACK; i++) {
		terrainTex[i] = texture(tex_terrainPack, vec3(fs_uv, i));
		normalTex[i] = texture(tex_normalPack, vec3(fs_uv, i)).rgb;
		specularTex[i] = texture(tex_specularPack, vec3(fs_uv, i));
	}

	returnColor = terrainTex[0] * alphaTex.r + terrainTex[1] * alphaTex.g + terrainTex[2] * alphaTex.b + terrainTex[3] * alphaTex.a;
	returnColor.w = 1;
	tex_normal = normalTex[0] * alphaTex.r + normalTex[1] * alphaTex.g + normalTex[2] * alphaTex.b + normalTex[3] * alphaTex.a;
	tex_normal = normalize(tex_normal);
	color_specular = specularTex[0] * alphaTex.r + specularTex[1] * alphaTex.g + specularTex[2] * alphaTex.b + specularTex[3] * alphaTex.a;
	return returnColor;
}

vec4 GetTextureColor_NoUse() {
	vec4 returnColor;
	vec4 alphaTex = texture(tex_alphamap, vs_uv);
	alphaTex.rg *= 255;

	vec4 terrain_A = texture(tex_terrainPack, vec3(fs_uv, int(alphaTex.r)));
	vec4 terrain_B = texture(tex_terrainPack, vec3(fs_uv, int(alphaTex.g)));
	vec3 normal_A = texture(tex_normalPack, vec3(fs_uv, int(alphaTex.r))).xyz;
	vec3 normal_B = texture(tex_normalPack, vec3(fs_uv, int(alphaTex.g))).xyz;
	vec4 specular_A = texture(tex_normalPack, vec3(fs_uv, int(alphaTex.r)));
	vec4 specular_B = texture(tex_normalPack, vec3(fs_uv, int(alphaTex.g)));
	returnColor = mix(terrain_A, terrain_B, alphaTex.b);
	tex_normal = mix(normal_A, normal_B, alphaTex.b);
	color_specular = mix(specular_A, specular_B, alphaTex.b);
	return returnColor;
}

vec3 CalcNewNormal(vec3 fs_normal) {
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

float PCF() {
	int sampleNum = 3;
	float shadowCount = 0.0f;
	float texWidth = 1.0f / 6400.0f;
	for(int i = -sampleNum; i < sampleNum + 1; i++) {
		for(int j = -sampleNum; j < sampleNum + 1; j++) {
			if(texture(tex_shadowmap, vec2(vs_shadow.x + i * texWidth, vs_shadow.y + j * texWidth)).x < vs_shadow.z)
				shadowCount += 1.0f;
		}
	}
	return 1.5f - shadowCount / pow(sampleNum * 2 + 1, 2);
}

void main() {
	
	vec4 texColor = GetTextureColor();

	if(fastMode == false) {
		vec3 lightToPos_vector[MAX_NUM_LIGHT];
		float lightToPos_distance[MAX_NUM_LIGHT];
		vec3 lightReflect_vector[MAX_NUM_LIGHT];
		float diffuse_factor = 0.0f;
		float specular_factor = 0.0f;
		vec3 fs_normal = normalize(vs_normal);
		fs_normal = CalcNewNormal(fs_normal);
		vec3 temp_vec3;

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
		float bias = 0.005f * tan(acos(diffuse_factor));
		bias = clamp(bias, 0, 0.0001f);

		finalColor = texColor * (coef_ambient + diffuse_factor) + specular_factor * color_specular * const_specular;
		finalColor.a = 1;

		float shadowDepth = texture(tex_shadowmap, vs_shadow.xy).x;
		if(vs_shadow.z - bias > shadowDepth) {
			if(vs_shadow.y < 1 && vs_shadow.y > -1 && vs_shadow.x < 1 && vs_shadow.x > -1)
				finalColor.xyz *= PCF();
		}

		//finalColor = mix(vec4(color_fog, 1), finalColor, visibility);
	}
	else {
		finalColor = texColor;
		finalColor.xyz *= 0.6f;
	}
}