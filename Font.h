#ifndef __FONT_H
#define __FONT_H

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "Mesh_Quad.h"

struct FontInfo {
	glm::vec2 uv;
	glm::vec2 size;
	glm::vec2 offset;
	glm::vec2 newOffset;
	int xAdvance;
	int page;
	int channel;
	Mesh_Quad mesh;
	GLuint vao;
	GLuint vbo_vertex;
	GLuint vbo_uv;

	FontInfo() {};
	~FontInfo() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo_vertex);
		glDeleteBuffers(1, &vbo_uv);
	}
};

struct Kerning {
	std::map<int, int> spacing;
};

class Font {
public:
	Font();
	~Font();

	void LoadTextureArray(const char *filePath, int z);
	void ReadFile(std::string filePath);
	GLuint GetVao(const int &asciiId);

	std::string filePath;
	std::string name;
	int fontSize;
	glm::vec4 padding; // up right down left
	glm::vec2 spacing;
	int lineHeight;
	int base;
	glm::vec2 scale;
	int numPages;
	int numChar;
	std::vector<std::string> pageNames;
	GLuint tbo_font;
	std::map<int, FontInfo> fontInfo;
	int numKernings;
	std::map<int, Kerning> kernings;
	bool hasKernings;

private:
	void GenObjectData(const int &index);
};
#endif