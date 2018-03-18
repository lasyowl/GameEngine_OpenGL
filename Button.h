#ifndef __BUTTON_H
#define __BUTTON_H

#include <gl/glew.h>
#include <string>
#include "Mesh_Quad.h"

class Button {
public:
	Button();
	Button(const glm::vec2 &origin, const glm::vec2 &size);
	~Button();

	void SetSize(const glm::vec2 &size);
	void SetOrigin(const glm::vec2 &origin);
	void SetColor(const glm::vec4 &color);
	void SetTexPack(const int &texPackDepth);
	void Initiate();
	GLuint GetVao();
	glm::vec2 GetOrigin();
	bool IsUnderMouse(const glm::vec2 &coord);
	void Highlight(const bool &highlight);
	void OnClick();
	void Activate();
	void Deactivate();

	GLuint tbo_background;
	GLuint tbo_border;
	glm::vec4 color_background;
	bool useBorder;
	bool underMouse;
	bool isActive;
	int buttonId;
	bool highlight;
	int texPackNum; // If numTexPack >= 0, use Sampler2DArray

private:
	void GenMesh();
	void GenObjects();

	glm::vec2 origin; // Left-down point coord
	glm::vec2 size;
	GLuint vao;
	GLuint vbo_vertex;
	GLuint vbo_uv;
	Mesh_Quad mesh;
};
#endif