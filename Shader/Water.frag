#version 430 core

#define MAX_NUM_LIGHT 100

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec2 vs_uv;
layout(location = 4) in vec3 posToEye_vector;
layout(location = 6) in float visibility;
layout(location = 8) in vec4 vs_clip;

layout(location = 0) out vec4 finalColor;

uniform int numLight = 0;
uniform float light_intensity[MAX_NUM_LIGHT];
uniform vec3 light_position[MAX_NUM_LIGHT];
uniform vec3 light_direction[MAX_NUM_LIGHT];
uniform vec3 color_specular = vec3(0.7f, 0.7f, 0.7f);
uniform float exponent_specular = 5.0f;

uniform vec3 color_fog = vec3(0.5f, 0.6f, 0.8f);

uniform sampler2D tex_reflect;
uniform sampler2D tex_trans;
uniform sampler2D tex_depth;
uniform sampler2D tex_dudv;
uniform sampler2D tex_normal;

uniform float moveFactor;

const float distortFactor = 0.1f;

void main() {
	vec2 ndc = vs_clip.xy / vs_clip.w / 2.0f + 0.5f;

	vec2 transTex = vec2(ndc.x, ndc.y);
	vec2 reflectTex = vec2(ndc.x, -ndc.y);

	float near = 0.1f;
	float far = 1000.0f;
	float depth = texture(tex_depth, transTex).r;
	float floorDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));
	depth = gl_FragCoord.z;
	float waterDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));
	float waterDepth = floorDistance - waterDistance;

	vec2 distortionCoord = texture(tex_dudv, vec2(vs_uv.x + moveFactor, vs_uv.y)).rg * 0.1f;
	distortionCoord = vs_uv + vec2(distortionCoord.x, distortionCoord.y + moveFactor);
	vec2 distortion = (texture(tex_dudv, distortionCoord).rg * 2.0f - 1.0f) * distortFactor * clamp(waterDepth / 10.0f, 0, 1);

	transTex += distortion;
	reflectTex += distortion;
	transTex = clamp(transTex, 0.001f, 0.999f);
	reflectTex.x = clamp(reflectTex.x, 0.001f, 0.999f);
	reflectTex.y = clamp(reflectTex.y, -0.999f, -0.001f);

	vec4 transColor = texture(tex_trans, transTex);
	vec4 reflectColor = texture(tex_reflect, reflectTex);
	
	vec4 normalColor = texture(tex_normal, distortionCoord);
	vec3 normal = vec3(normalColor.r * 2.0f - 1.0f, normalColor.b * 1.5f, normalColor.g * 2.0f - 1.0f);
	normal = normalize(normal);
	
	vec3 viewVector = normalize(posToEye_vector);
	float refractFactor = dot(viewVector, vec3(0,1,0));
	refractFactor = pow(refractFactor, 2.0f);

	vec3 lightToPos_vector[MAX_NUM_LIGHT];
	float lightToPos_distance[MAX_NUM_LIGHT];
	vec3 lightReflect_vector[MAX_NUM_LIGHT];
	float specular_factor = 0.0f;
	for(int i = 0; i < numLight; i++) {
		lightToPos_vector[i] = normalize(vs_position - light_position[i]);
		lightToPos_distance[i] = distance(vs_position, light_position[i]);
		lightReflect_vector[i] = reflect(lightToPos_vector[i], normal);
		specular_factor += max(pow(dot(lightReflect_vector[i], viewVector), exponent_specular), 0.0f) * light_intensity[i] / pow(lightToPos_distance[i], 2);
	}

	finalColor = mix(reflectColor, transColor, refractFactor);
	vec4 waterColor = vec4(0, 0.1f, 0.2f, 1.0f);

	finalColor = mix(finalColor, waterColor, 0.2f) + 0.3f * vec4(color_specular * specular_factor * clamp(waterDepth / 2.0f, 0, 1), 0);
	finalColor.a = clamp(waterDepth * 10.0f, 0, 1);
	//finalColor = mix(vec4(color_fog, 1), finalColor, visibility);
}