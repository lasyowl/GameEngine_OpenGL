#include <time.h>
#include <GL/glew.h>
#include <FreeImage/FreeImage.h>
#include "Render.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

Render::Render() : varRender(nullptr) {
	parseShader = new Parse_Shader;
}

Render::~Render() {
	if (varRender != nullptr)
		delete varRender;
	delete parseShader;
}

void Render::InitRenderer() {
	GenDefaultShaderProgram();
}

void Render::GenDefaultShaderProgram() {

	ShaderInfo shaderInfo[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Default.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Default.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo));

	GetDefaultShaderVar();
}

void Render::GetDefaultShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
}

void Render::LoadTexture(const char *filePath) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLubyte *data;

	if (*filePath == NULL) {
		return;
	}
	tx_file_format = FreeImage_GetFileType(filePath, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filePath);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

void Render::LoadTextureArray(const char *filePath, int z) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLubyte *data;

	if (*filePath == NULL) {
		return;
	}
	tx_file_format = FreeImage_GetFileType(filePath, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filePath);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, z, width, height, 1, GL_BGRA, GL_UNSIGNED_BYTE, data);
	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, z, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

void Render::GenRandomTexture(int size) {
	std::vector<glm::vec3> data;
	data.resize(size);
	srand(clock());
	float adjRand = 1.0f / RAND_MAX;
	for (int i = 0; i < size; i++) {
		data[i].r = rand() * adjRand;
		data[i].g = rand() * adjRand;
		data[i].b = rand() * adjRand;
	}
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, &data[0][0]);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}