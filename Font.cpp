#include "Font.h"
#include <fstream>
#include <FreeImage/FreeImage.h>

using namespace std;
using namespace glm;

Font::Font() : hasKernings(false) {
	filePath = "Res/Texture/UI/Text/";
}

Font::~Font() {
	glDeleteTextures(1, &tbo_font);
}

void Font::LoadTextureArray(const char *filePath, int z) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLvoid *data;

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

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);

	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n", width, height);
}

void Font::ReadFile(string fileName) {
	fstream file;
	file.open(filePath + fileName);
	char buffer[100];
	char buffer_s[100];

	file >> buffer >> buffer;
	sscanf_s(buffer, "face=\"%s\"", buffer_s);
	name = buffer_s;
	name[name.size() - 1] = '\0';

	file >> buffer;
	sscanf_s(buffer, "size=%d", &fontSize);

	file >> buffer >> buffer >> buffer >> buffer >> buffer >> buffer >> buffer >> buffer;
	sscanf_s(buffer, "padding=%f,%f,%f,%f", &padding[0], &padding[1], &padding[2], &padding[3]);

	file >> buffer;
	sscanf_s(buffer, "spacing=%f,%f", &spacing[0], &spacing[1]);

	file >> buffer >> buffer >> buffer;
	sscanf_s(buffer, "lineHeight=%d", &lineHeight);

	file >> buffer;
	sscanf_s(buffer, "base=%d", &base);

	file >> buffer;
	sscanf_s(buffer, "scaleW=%f", &scale[0]);
	file >> buffer;
	sscanf_s(buffer, "scaleH=%f", &scale[1]);

	file >> buffer;
	sscanf_s(buffer, "pages=%d", &numPages);

	file.getline(buffer, 200);
	pageNames.resize(numPages);
	glGenTextures(numPages, &tbo_font);
	glActiveTexture(GL_TEXTURE0 + tbo_font);
	glBindTexture(GL_TEXTURE_2D_ARRAY, tbo_font);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, scale.x, scale.y, numPages);
	for (int i = 0; i < numPages; i++) {
		file >> buffer >> buffer >> buffer;
		sscanf_s(buffer, "file=\"%s\"", buffer_s);
		pageNames[i] = buffer_s;
		pageNames[i][pageNames[i].size() - 1] = '\0';

		LoadTextureArray((filePath + pageNames[i]).data(), i);
	}
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	
	file >> buffer >> buffer;
	sscanf_s(buffer, "count=%d", &numChar);
	for (int i = 0; i < numChar; i++) {
		int asciiId;
		file >> buffer >> buffer;
		sscanf_s(buffer, "id=%d", &asciiId);

		file >> buffer;
		sscanf_s(buffer, "x=%f", &fontInfo[asciiId].uv.x);
		file >> buffer;
		sscanf_s(buffer, "y=%f", &fontInfo[asciiId].uv.y);

		file >> buffer;
		sscanf_s(buffer, "width=%f", &fontInfo[asciiId].size.x);
		file >> buffer;
		sscanf_s(buffer, "height=%f", &fontInfo[asciiId].size.y);

		file >> buffer;
		sscanf_s(buffer, "xoffset=%f", &fontInfo[asciiId].offset.x);
		file >> buffer;
		sscanf_s(buffer, "yoffset=%f", &fontInfo[asciiId].offset.y);

		file >> buffer;
		sscanf_s(buffer, "xadvance=%d", &fontInfo[asciiId].xAdvance);

		file >> buffer;
		sscanf_s(buffer, "page=%d", &fontInfo[asciiId].page);

		file >> buffer;
		sscanf_s(buffer, "chnl=%d", &fontInfo[asciiId].channel);

		fontInfo[asciiId].newOffset.y = lineHeight - fontInfo[asciiId].size.y - fontInfo[asciiId].offset.y;
	}

	file >> buffer >> buffer;
	if (buffer[1] != 'o') {
		numKernings = 0; 
		map<int, FontInfo>::iterator iter;
		if (!fontInfo.empty()) {
			for (iter = fontInfo.begin(); iter != fontInfo.end(); ++iter) {
				GenObjectData(iter->first);
			}
		}
		file.close();
	}
	else {
		sscanf_s(buffer, "count=%d", &numKernings);
		hasKernings = true;
		for (int i = 0; i < numKernings; i++) {
			int buffer_i0, buffer_i1, buffer_i2;
			file >> buffer >> buffer;
			sscanf_s(buffer, "first=%d", &buffer_i1);

			Kerning temp;
			file >> buffer;
			sscanf_s(buffer, "second=%d", &buffer_i0);

			file >> buffer;
			sscanf_s(buffer, "amount=%d", &buffer_i2);
			temp.spacing[buffer_i1] = buffer_i2;

			if (kernings.count(buffer_i0) > 0)
				kernings[buffer_i0].spacing[buffer_i1] = buffer_i2;
			else kernings[buffer_i0] = temp;
		}
		map<int, FontInfo>::iterator iter;
		if (!fontInfo.empty()) {
			for (iter = fontInfo.begin(); iter != fontInfo.end(); ++iter) {
				GenObjectData(iter->first);
			}
		}
		file.close();
	}
}

void Font::GenObjectData(const int &index) {
	FontInfo *fontInfo = &this->fontInfo[index];
	fontInfo->mesh.SetVariables(vec2(0, 0), fontInfo->size);
	fontInfo->mesh.uv[1] = fontInfo->uv / scale;
	fontInfo->mesh.uv[0] = (fontInfo->uv + vec2(0, fontInfo->size.y)) / scale;
	fontInfo->mesh.uv[3] = (fontInfo->uv + vec2(fontInfo->size.x, 0)) / scale;
	fontInfo->mesh.uv[2] = (fontInfo->uv + fontInfo->size) / scale;
	fontInfo->mesh.uv[0].y = 1 - fontInfo->mesh.uv[0].y;
	fontInfo->mesh.uv[1].y = 1 - fontInfo->mesh.uv[1].y;
	fontInfo->mesh.uv[2].y = 1 - fontInfo->mesh.uv[2].y;
	fontInfo->mesh.uv[3].y = 1 - fontInfo->mesh.uv[3].y;

	glGenVertexArrays(1, &fontInfo->vao);
	glGenBuffers(1, &fontInfo->vbo_vertex);
	glGenBuffers(1, &fontInfo->vbo_uv);

	glBindVertexArray(fontInfo->vao);

	glBindBuffer(GL_ARRAY_BUFFER, fontInfo->vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, fontInfo->mesh.vertex.size() * sizeof(vec3), &fontInfo->mesh.vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, fontInfo->vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, fontInfo->mesh.uv.size() * sizeof(vec2), &fontInfo->mesh.uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);
}

GLuint Font::GetVao(const int &asciiId) {
	return fontInfo[asciiId].vao;
}