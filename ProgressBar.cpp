#include "ProgressBar.h"

using namespace glm;

ProgressBar::ProgressBar() : mode(PROGRESS_MODE), isActive(false), propertyAttached(false), progress(0.5f) {
	origin = vec2(0, 0);
	size = vec2(50, 50);
	color[PB_BACKGROUND] = vec4(1, 1, 1, 1);
	color[PB_LEFTBAR] = vec4(1, 1, 1, 1);
	color[PB_EFFECT] = vec4(1, 1, 1, 1);
	color[PB_CURSOR] = vec4(0.3f, 0.6f, 1.0f, 1);
}

ProgressBar::~ProgressBar() {
	glDeleteVertexArrays(NUM_PB_MESH, vao);
	glDeleteBuffers(NUM_PB_MESH, vbo_vertex);
	glDeleteBuffers(NUM_PB_MESH, vbo_uv);
}

void ProgressBar::SetSize(const float &x, const float &y) {
	size.x = x;
	size.y = y;
}

void ProgressBar::SetOrigin(const float &x, const float &y) {
	origin.x = x;
	origin.y = y;
}

void ProgressBar::SetColor(const int &target, const glm::vec4 &color) {
	this->color[target] = color;
}

void ProgressBar::SetMode(const int &mode) {
	this->mode = mode;
}

void ProgressBar::Initiate() {
	GenMesh();
	GenObjects();
}

void ProgressBar::Activate() {
	isActive = true;
}

void ProgressBar::Deactivate() {
	isActive = false;
}

GLuint ProgressBar::GetVao(const int &target) {
	return vao[target];
}

void ProgressBar::SetProgress(const float &progress) {
	this->progress = progress;
}

void ProgressBar::SetTexture(ProgressBar_Tex *pBar_tex) {
	this->pBar_tex = pBar_tex;
}

void ProgressBar::AttachProperty(float *target, const float &minimum, const float &maximum) {
	propertyAttached = true;
	attachedProperty = target;
	attachedProperty_min = minimum;
	attachedProperty_max = maximum;
	progress = clamp((*attachedProperty - attachedProperty_min) / (attachedProperty_max - attachedProperty_min), 0.0f, 1.0f);
}

void ProgressBar::DetachProperty() {
	propertyAttached = false;
	attachedProperty = nullptr;
}

float ProgressBar::CalcProperty() {
	*attachedProperty = attachedProperty_min + (attachedProperty_max - attachedProperty_min) * progress;
	return *attachedProperty;
}

bool ProgressBar::OnCursorTest(const glm::vec2 mouseCoord) {
	float sFactor = size.x * progress * 0.95f;
	vec3 v0 = mesh[PB_CURSOR].vertex[0] + vec3(sFactor, 0, 0);
	vec3 v1 = mesh[PB_CURSOR].vertex[3] + vec3(sFactor, 0, 0);
	if (mouseCoord.x > v0.x && mouseCoord.x < v1.x
		&& mouseCoord.y > v0.y && mouseCoord.y < v1.y) {
		return true;
	}
	return false;
}

void ProgressBar::GenMesh() {
	mesh[PB_BACKGROUND].SetVariables(origin, size);
	mesh[PB_LEFTBAR].SetVariables(origin, size);
	mesh[PB_EFFECT].SetVariables(origin, size);
	mesh[PB_CURSOR].SetVariables(origin, vec2(size.x * 0.05f, size.y));
}

void ProgressBar::GenObjects() {
	glGenVertexArrays(NUM_PB_MESH, vao);
	glGenBuffers(NUM_PB_MESH, vbo_vertex);
	glGenBuffers(NUM_PB_MESH, vbo_uv);

	for (int i = 0; i < NUM_PB_MESH; i++) {
		glBindVertexArray(vao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].vertex.size() * sizeof(vec3), &mesh[i].vertex[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].uv.size() * sizeof(vec2), &mesh[i].uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}
}
