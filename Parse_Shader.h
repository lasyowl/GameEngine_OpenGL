#ifndef __PARSE_SHADER_H
#define __PARSE_SHADER_H

#define MAX_SHADER_NUMBER 4

typedef unsigned int GLenum, GLuint;

struct ShaderInfo {
	GLenum type;
	const char *filename;
	GLuint shader;
};

class Parse_Shader {
public:
	Parse_Shader() {}

	GLuint LoadShader(ShaderInfo *const shaderInfo);
};

#endif