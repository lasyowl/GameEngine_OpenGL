#ifndef __VAR_RENDER_H
#define __VAR_RENDER_H

#include <glm/glm.hpp>

typedef unsigned int GLuint;

class Var_Render {

public:
	Var_Render();
	~Var_Render();

	GLuint loc_ModelMatrix;
	GLuint loc_ViewMatrix;
	GLuint loc_ProjectionMatrix;
	GLuint loc_ViewProjectionMatrix;
	GLuint loc_ModelViewProjectionMatrix;
	GLuint loc_InvTransposeMatrix;
	GLuint loc_GlobalInvMatrix;
	GLuint loc_BoneMatrix;
	GLuint loc_useBone;

	GLuint loc_texture_diffuse;
	GLuint loc_texture_normalmap;
	GLuint loc_texture_array;
	GLuint loc_color_ambient;
	GLuint loc_color_diffuse;
	GLuint loc_color_specular;
	GLuint loc_color_line;
	GLuint loc_useTexture;
	GLuint loc_useNormalmap;
	GLuint loc_numLight;
	GLuint loc_light_intensity;
	GLuint loc_light_position;
	GLuint loc_light_direction;
	GLuint loc_eye_position;
	GLuint loc_heightmapWidthHeight;
	GLuint loc_tex_terrainPack;
	GLuint loc_tex_normalPack;
	GLuint loc_tex_specularPack;
	GLuint loc_tex_alphamap;

	GLuint loc_tex_reflect;
	GLuint loc_tex_trans;
	GLuint loc_tex_depth;
	GLuint loc_tex_dudv;
	GLuint loc_tex_normal;
	GLuint loc_moveFactor;

	GLuint loc_tex_shadowmap;
	GLuint loc_shadowMatrix;
	GLuint loc_tex_background;
	GLuint loc_tex_border;
	GLuint loc_useBorder;
	GLuint loc_highlight;
	GLuint loc_color_background;
	GLuint loc_color_text;
	GLuint loc_tex_text;
	GLuint loc_textPage;
	GLuint loc_textOffset;
	GLuint loc_boxOffset;
	GLuint loc_fontSize;
	GLuint loc_progress;
	GLuint loc_originX;
	GLuint loc_sizeX;
	GLuint loc_marker;
	GLuint loc_intensity;
	GLuint loc_tex_framebuffer;
	GLuint loc_fastMode;
	GLuint loc_tex_backgroundPack;
	GLuint loc_tex_gblur;
	GLuint loc_state_gblur;

	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewProjectionMatrix;
	glm::mat4 ModelViewProjectionMatrix;
	glm::mat3 InvTransposeMatrix;
	glm::mat4 GlobalInvMatrix;
	glm::mat4 ShadowMatrix;
	glm::mat4 *BoneMatrix;
	int numBoneMatrix;
	bool useBone;
	float intensity;
	glm::vec3 eyePosition;
	glm::vec3 color_line;

private:
};

#endif