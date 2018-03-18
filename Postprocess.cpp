#include "Postprocess.h"
#include "Render_Postprocess.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

Postprocess::Postprocess() : intensity(0.0f), finalize(false) {

}

Postprocess::~Postprocess() {

}

void Postprocess::SetViewport(vec2 *viewport) {
	this->viewport = viewport;
}

void Postprocess::SetRenderer(Render_Postprocess *renderer) {
	this->renderer = renderer;
}

void Postprocess::SetProcessState(const int &process, const int &state) {
	processState[process] = state;
}

void Postprocess::SetAdditionalTexture(const GLuint &texture_ext, const int &target) {
	renderer->SetTexture_Ext(texture_ext, target);
}

void Postprocess::Initiate() {
	fullQuad.SetVariables(vec2(0), *viewport);
	VP_FullQuad = ortho(0.0f, viewport->x, 0.0f, viewport->y, -1.0f, 2.0f) * lookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0));
	
	GenObject();
}

int Postprocess::RunProcess(const GLuint &texture_fbo, const int &process) {
	switch (process) {
		case POSTPROCESS_DEFAULT:
			return Process_Default(texture_fbo);
			break;
		case POSTPROCESS_GBLUR:
			return Process_GaussianBlur(texture_fbo);
			break;
		default:
			break;
	}
}

void Postprocess::Finalize(const bool &tf) {
	finalize = tf;
}

void Postprocess::GenObject() {
	vao.resize(POSTPROCESS_SIZE);
	vbo_vertex.resize(POSTPROCESS_SIZE);
	vbo_uv.resize(POSTPROCESS_SIZE);
	processState.resize(POSTPROCESS_SIZE, PROCESS_INITIATING);

	glGenVertexArrays(POSTPROCESS_SIZE, &vao[0]);
	glGenBuffers(POSTPROCESS_SIZE, &vbo_vertex[0]);
	glGenBuffers(POSTPROCESS_SIZE, &vbo_uv[0]);

	for (int i = 0; i < POSTPROCESS_SIZE; i++) {
		glBindVertexArray(vao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[i]);
		glBufferData(GL_ARRAY_BUFFER, fullQuad.vertex.size() * sizeof(vec3), &fullQuad.vertex[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[i]);
		glBufferData(GL_ARRAY_BUFFER, fullQuad.uv.size() * sizeof(vec2), &fullQuad.uv[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}
}

int Postprocess::Process_Default(const GLuint &texture_fbo) {
	renderer->SetShader(SHADER_PP_DEFAULT);
	renderer->SetTexture(texture_fbo);
	renderer->SetVP(VP_FullQuad);
	renderer->SetVAO(vao[POSTPROCESS_DEFAULT]);
	renderer->SetIntensity(intensity);
	renderer->PrepareDraw();
	renderer->Draw();
	renderer->FinishDraw();

	int result;
	switch (processState[POSTPROCESS_DEFAULT]) {
	case PROCESS_INITIATING:
		if (intensity >= 1.0f)
			processState[POSTPROCESS_DEFAULT] = PROCESS_INITIATED;
		else {
			intensity += 0.01f;
		}
		break;
	case PROCESS_FINISHING:
		if (intensity < 0.0f) {
			processState[POSTPROCESS_DEFAULT] = PROCESS_FINISHED;
			result = processState[POSTPROCESS_DEFAULT];
			processState[POSTPROCESS_DEFAULT] = PROCESS_INITIATING;
			return result;
		}
		else {
			intensity -= 0.01f;
		}
		break;
	default:
		break;
	}
	if (finalize == true) {
		return processState[POSTPROCESS_DEFAULT];
	}
	else {
		return 0;
	}
}

int Postprocess::Process_GaussianBlur(const GLuint &texture_fbo) {
	renderer->SetShader(SHADER_PP_GBLUR);

	switch (processState[POSTPROCESS_GBLUR]) {
	case PROCESS_INITIATING:
		renderer->SetShaderState(GBLUR_BRIGHT);
		processState[POSTPROCESS_GBLUR] = PROCESS_INITIATED;
		break;
	case PROCESS_INITIATED:
		renderer->SetShaderState(GBLUR_VERT);
		processState[POSTPROCESS_GBLUR] = PROCESS_FINISHING;
		break;
	case PROCESS_FINISHING:
		renderer->SetShaderState(GBLUR_HORIZ);
		processState[POSTPROCESS_GBLUR] = PROCESS_FINISHED;
		break;
	case PROCESS_FINISHED:
		renderer->SetShaderState(GBLUR_FINAL);
		processState[POSTPROCESS_GBLUR] = PROCESS_INITIATING;
		break;
	default:
		break;
	}

	renderer->SetTexture(texture_fbo);
	renderer->SetVP(VP_FullQuad);
	renderer->SetVAO(vao[POSTPROCESS_GBLUR]);
	renderer->SetIntensity(intensity);
	renderer->PrepareDraw();
	renderer->Draw();
	renderer->FinishDraw();

	return 0;
}
