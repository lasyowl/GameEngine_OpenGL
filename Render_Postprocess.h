#ifndef __RENDER_POSTPROCESS
#define __RENDER_POSTPROCESS

#include <GL/glew.h>
#include "Render.h"

enum Shader_Postprocess { SHADER_PP_DEFAULT, SHADER_PP_GBLUR, SHADER_PP_SIZE };
enum Texture_Postprocess { PP_TEX_GBLUR };
enum GBlur_State { GBLUR_BRIGHT, GBLUR_VERT, GBLUR_HORIZ, GBLUR_FINAL };

class Render_Postprocess : public Render {
public:
	Render_Postprocess();
	~Render_Postprocess();

	void Initiate();
	void SetShader(const int &program);
	void SetVP(const glm::mat4 &VP);
	void SetVAO(const GLuint &vao);
	void SetIntensity(const float &intensity);
	void SetTexture(const GLuint &texture_fbo);
	void SetTexture_Ext(const GLuint &texture_ext, const int &target);
	void SetShaderState(const int &state);
	void PrepareDraw();
	void Draw();
	void FinishDraw();

private:
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();

	GLuint vao;
	GLuint tex_framebuffer;
	Var_Render *varRender_GBlur;
	int currentShader;
};
#endif