#include "Button.h"

using namespace std;
using namespace glm;

Button::Button() : useBorder(false), underMouse(false), tbo_background(-1), tbo_border(-1), isActive(false), 
	highlight(false), texPackNum(-1) {
	origin = vec2(0, 0);
	size = vec2(50, 50);
	color_background = vec4(1);
}

Button::Button(const glm::vec2 &origin, const glm::vec2 &size) : useBorder(false), underMouse(false), tbo_background(-1), 
	tbo_border(-1), isActive(false), highlight(false), texPackNum(-1) {
	this->origin = origin;
	this->size = size;
}

Button::~Button() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteBuffers(1, &vbo_uv);
	glDeleteTextures(1, &tbo_background);
	glDeleteTextures(1, &tbo_border);
	GLuint tbo_background;
	GLuint tbo_border;
}

void Button::SetColor(const vec4 &color) {
	this->color_background = color;
}

void Button::SetTexPack(const int &texPackDepth) {
	this->texPackNum = texPackDepth;
}

void Button::SetSize(const vec2 &size) {
	this->size = size;
}

void Button::SetOrigin(const vec2 &origin) {
	this->origin = origin;
}

void Button::Initiate() {
	GenMesh();
	GenObjects();
}

void Button::GenMesh() {
	mesh.SetVariables(origin, size);
}

void Button::Activate() {
	isActive = true;
}

void Button::Deactivate() {
	isActive = false;
}

void Button::GenObjects() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_uv);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertex.size() * sizeof(vec3), &mesh.vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, mesh.uv.size() * sizeof(vec2), &mesh.uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);
}

GLuint Button::GetVao() {
	return vao;
}

vec2 Button::GetOrigin() {
	return origin;
}

bool Button::IsUnderMouse(const vec2 &coord) {
	if (coord.x > mesh.vertex[0].x && coord.x < mesh.vertex[3].x
		&& coord.y > mesh.vertex[0].y && coord.y < mesh.vertex[3].y) {
		underMouse = true;
		return true;
	}
	underMouse = false;
	return false;
}

void Button::OnClick() {

}

void Button::Highlight(const bool &highlight) {
	this->highlight = highlight;
}