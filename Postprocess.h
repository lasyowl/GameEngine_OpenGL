#ifndef __POSTPROCESS_H
#define __POSTPROCESS_H

#include <vector>
#include <gl/glew.h>
#include "Mesh_Quad.h"

class Render_Postprocess;

enum ProcessList { POSTPROCESS_DEFAULT, POSTPROCESS_GBLUR, POSTPROCESS_SIZE };
enum ProcessState { PROCESS_INITIATING, PROCESS_INITIATED ,PROCESS_FINISHING, PROCESS_FINISHED };

class Postprocess {
public:
	Postprocess();
	~Postprocess();

	void Initiate();

	void SetViewport(glm::vec2 *viewport);
	void SetRenderer(Render_Postprocess *renderer);
	void SetProcessState(const int &process, const int &state);
	void SetAdditionalTexture(const GLuint &texture_ext, const int &target);
	int RunProcess(const GLuint &texture_fbo, const int &process);
	void Finalize(const bool &tf);

private:
	void GenObject();
	int Process_Default(const GLuint &texture_fbo);
	int Process_GaussianBlur(const GLuint &texture_fbo);

	Render_Postprocess *renderer;
	Mesh_Quad fullQuad;
	glm::vec2 *viewport;
	glm::mat4 VP_FullQuad;
	std::vector<GLuint> vao;
	std::vector<GLuint> vbo_vertex;
	std::vector<GLuint> vbo_uv;
	std::vector<int> processState;
	float intensity;
	bool finalize;
};
#endif