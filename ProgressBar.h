#ifndef __PROGRESS_BAR_H
#define __PROGRESS_BAR_H

#include "Mesh_Quad.h"
#include <gl/glew.h>

#ifndef NUM_PB_MESH
#define NUM_PB_MESH 4
#endif

class ProgressBar_Tex;

enum ProgressBarIndex { PB_BACKGROUND, PB_LEFTBAR, PB_EFFECT, PB_CURSOR };
enum AdjustMode { PROGRESS_MODE, ADJUST_MODE };

class ProgressBar {
public:
	ProgressBar();
	~ProgressBar();

	void SetSize(const float &x, const float &y);
	void SetOrigin(const float &x, const float &y);
	void SetColor(const int &target, const glm::vec4 &color);
	void SetMode(const int &mode);
	void SetProgress(const float &progress);
	void SetTexture(ProgressBar_Tex *pBar_tex);
	bool OnCursorTest(const glm::vec2 mouseCoord);
	void AttachProperty(float *target, const float &minimum, const float &maximum);
	void DetachProperty();
	float CalcProperty();
	void Initiate();
	void Activate();
	void Deactivate();
	GLuint GetVao(const int &target);

	glm::vec4 color[NUM_PB_MESH];
	glm::vec2 origin;
	glm::vec2 size;
	int mode;
	bool isActive;
	float progress;
	ProgressBar_Tex *pBar_tex;
	bool propertyAttached;
	float attachedProperty_min, attachedProperty_max;
	float *attachedProperty;

private:
	void GenMesh();
	void GenObjects();

	Mesh_Quad mesh[NUM_PB_MESH];
	GLuint vao[NUM_PB_MESH];
	GLuint vbo_vertex[NUM_PB_MESH];
	GLuint vbo_uv[NUM_PB_MESH];
};
#endif