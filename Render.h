#ifndef __RENDER_H
#define __RENDER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Var_Render;
class Parse_Shader;
struct ShaderInfo;

class Render {
public:
	Render();
	virtual ~Render();

	void InitRenderer();
	void LoadTexture(const char *filePath);
	void LoadTextureArray(const char *filePath, int z);

protected:
	void GenRandomTexture(int size);

	Parse_Shader *parseShader;
	std::vector<GLuint> shaderProgram;
	Var_Render *varRender;
	Var_Render *varRender_selectionBox;

private:
	void GenDefaultShaderProgram();
	void GetDefaultShaderVar();
};
#endif