#include "Sun.h"
#include "Loader_Texture.h"

#include <string>

using namespace std;
using namespace glm;

Sun::Sun() : position(nullptr) {

}

Sun::~Sun() {

}

void Sun::SetPosition(vec3 *position) {
	this->position = position;
}

void Sun::Initiate() {
	float size = 2000.0f;
	mesh.SetVariables(vec2(-size / 2), vec2(size));
	GenObjects();
	BindObjectData();
}

void Sun::GenObjects() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_uv);
	glGenTextures(1, &tex_core);
	
	glGenTextures(9, tex_lens);
}

void Sun::BindObjectData() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertex.size() * 3 * sizeof(float), &mesh.vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, mesh.uv.size() * 2 * sizeof(float), &mesh.uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);

	Loader_Texture loader;
	glActiveTexture(GL_TEXTURE0 + tex_core);
	glBindTexture(GL_TEXTURE_2D, tex_core);
	loader.LoadTexture("Res/Texture/Sun/sun.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	for (int i = 0; i < 9; i++) {

		string filePath = "Res/Texture/Sun/tex";
		char fileNum[10];
		sprintf_s(fileNum, sizeof(fileNum), "%d", i + 1);
		filePath += fileNum;
		filePath += ".png";

		Loader_Texture loader;
		glActiveTexture(GL_TEXTURE0 + tex_lens[i]);
		glBindTexture(GL_TEXTURE_2D, tex_lens[i]);
		loader.LoadTexture(filePath.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}