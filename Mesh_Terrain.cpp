#include "Mesh_Terrain.h"

#include <stdio.h>
#include <Windows.h>
#include <GL/glew.h>
#include <FreeImage/FreeImage.h>

#include "Var_Compute.h"
#include "Parse_Shader.h"

using namespace glm;
using namespace std;

Mesh_Terrain::Mesh_Terrain() : parse(nullptr), varCompute(nullptr) {
	filePath = "Res/Heightmap/";
	fileName_heightmap = "DinoIsland06.gif";
	fullPath = filePath + fileName_heightmap;
}

Mesh_Terrain::Mesh_Terrain(const int &width, const vec2 &offset) : parse(nullptr), varCompute(nullptr) { // Square : width = height
	filePath = "Res/Heightmap/";
	fileName_heightmap = "DinoIsland06.gif";
	fullPath = filePath + fileName_heightmap;
	SetDimension(width, offset);
}

Mesh_Terrain::~Mesh_Terrain() {
	if (parse != nullptr)
		delete parse;
	if (varCompute != nullptr)
		delete varCompute;

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteBuffers(1, &vbo_normal);
	glDeleteTextures(1, &tbo_heightmap);
}

void Mesh_Terrain::SetHeightmap(const string &fileName) {
	fileName_heightmap = fileName;
	fullPath = filePath + fileName_heightmap;
}

void Mesh_Terrain::SetDimension(const int &width, const vec2 &offset) {
	this->width = width;
	this->offset = offset;
}

void Mesh_Terrain::GenMeshVertex() {
	int width = this->width + 1;
	vertex.resize(pow(width, 2), vec4(0.0f));
	normal.resize(pow(width, 2), vec4(0.0f));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			vertex[i * width + j] = vec4(j + offset.x, 0.0f, -i + offset.y, 1.0f);
			normal[i * width + j] = vec4(0.0f);
		}
	}
}

void Mesh_Terrain::Initiate() {
	GenMeshVertex();
	GenVariables();
	GenObjects();
	GenShader();
	BindObjects();
	DispatchMesh();
}

GLuint Mesh_Terrain::GetVao() {
	return vao;
}

void Mesh_Terrain::GenVariables() {
	parse = new Parse_Shader;
	varCompute = new Var_Compute;
}

void Mesh_Terrain::GenObjects() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_normal);
	glGenTextures(1, &tbo_heightmap);
}

void Mesh_Terrain::BindObjects() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo_vertex);
	glBufferData(GL_SHADER_STORAGE_BUFFER, vertex.size() * sizeof(vec4), &vertex[0][0], GL_STATIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_normal);
	glBufferData(GL_SHADER_STORAGE_BUFFER, normal.size() * sizeof(vec4), &normal[0][0], GL_STATIC_DRAW);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vec4), &vertex[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(vec4), &normal[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0 + tbo_heightmap);
	glBindTexture(GL_TEXTURE_2D, tbo_heightmap);
	LoadTexture(fullPath.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Mesh_Terrain::DispatchMesh() {
	glUseProgram(shader_compute);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo_vertex);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_normal);

	glDispatchCompute(width + 1, width + 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo_vertex);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, vertex.size() * sizeof(vec4), &vertex[0][0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_normal);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, normal.size() * sizeof(vec4), &normal[0][0]);
}

void Mesh_Terrain::GenShader() {
	ShaderInfo shaderInfo_compute[MAX_SHADER_NUMBER] = {
		{ GL_COMPUTE_SHADER, "Shader/Terrain.comp" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shader_compute = parse->LoadShader(shaderInfo_compute);

	glUseProgram(shader_compute);
	varCompute->loc_texture_heightmap = glGetUniformLocation(shader_compute, "texture_heightmap");
	varCompute->loc_heightmapWidthHeight = glGetUniformLocation(shader_compute, "heightmapWidthHeight");
	glUniform1i(varCompute->loc_texture_heightmap, tbo_heightmap);
	glUniform1f(varCompute->loc_heightmapWidthHeight, width);
}

void Mesh_Terrain::LoadTexture(const char *filePath) {
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

void Mesh_Terrain::LoadTextureArray(const char *filePath, int z) {
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
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

vec3 Mesh_Terrain::GetRaycastCoord_MP(const vec3 &origin, const vec3 &dir, const float &range, const int &iteration) {
	float border = width;
	vec3 newOrigin = origin;
	float newRange = range;
	bool detected = false;
	for (int i = 0; i < iteration; i++) {
		vec3 checkPoint = newOrigin + dir * 0.5f * newRange;
		//printf("%f %f %f\n", checkPoint.x, checkPoint.y, checkPoint.z);
		newRange *= 0.5f;
		if (checkPoint.x > border || checkPoint.x < 0 || checkPoint.z < -border || checkPoint.z > 0) {
			//printf("err\n");
			continue;
		}

		if (checkPoint.y > GetHeight(checkPoint)) {
			newOrigin = checkPoint;
			detected = true;
		}
	}
	newOrigin.y = GetHeight(newOrigin);

	if (detected)
		return newOrigin;
	else return vec3(-1, -1, -1);
}

vec3 Mesh_Terrain::GetRaycastCoord_S(const vec3 &origin, const vec3 &dir, const float &delta, const float &range) {
	float border = width - 1.0f;
	bool detected = false;
	vec3 newOrigin = origin;
	vec3 deltaRay = delta * dir;
	int iteration = range / delta;
	for (int i = 0; i < iteration; i++) {
		newOrigin += deltaRay;
		//printf("%f %f %f\n", newOrigin.x, newOrigin.y, newOrigin.z);
		if (newOrigin.x > border || newOrigin.x < 0 || newOrigin.z < -border || newOrigin.z > 0) {
			continue;
		}
		if (GetHeight(newOrigin) > newOrigin.y) {
			newOrigin -= deltaRay;
			newOrigin.y = GetHeight(newOrigin);
			return newOrigin;
		}
	}
	return vec3(ERR_RAYCAST, 0, 0);
}

float Mesh_Terrain::GetHeight(const vec3 &objectPosition) {
	int index_LD = (int)-objectPosition.z * (width + 1) + (int)objectPosition.x;
	int index_LU = index_LD + width + 1;
	int index_RU = index_LU + 1;
	int index_RD = index_LD + 1;
	vec3 coord_LD = vec3(vertex[index_LD]);
	vec3 coord_LU = vec3(vertex[index_LU]); 
	vec3 coord_RU = vec3(vertex[index_RU]);
	vec3 coord_RD = vec3(vertex[index_RD]);

	float pos_x = objectPosition.x - (int)objectPosition.x;
	float pos_z = -objectPosition.z - (int)(-objectPosition.z);

	float height_LD = coord_LD.y;
	float height_RU = coord_RU.y;
	float height_LU = coord_LU.y;
	float height_RD = coord_RD.y;

	float interpolatedHeight;

	if (pos_x + pos_z <= 1.0f) {
		float a1 = height_LD * (1 - pos_x) + height_RD * pos_x;
		float a2 = height_LU * (1 - pos_x) + height_RD * pos_x;
		interpolatedHeight = a2 * pos_z / (1 - pos_x) + a1 * (1 - (pos_z / (1 - pos_x)));
	}
	else {
		float a1 = height_LU * (1 - pos_x) + height_RU * pos_x;
		float a2 = height_LU * (1 - pos_x) + height_RD * pos_x;
		interpolatedHeight = a2 * (1 - pos_z) / pos_x + a1 * (1 - (1 - pos_z) / pos_x);
	}
	/*printf("%f %f\n", pos_x, pos_z);
	printf("%f\n", interpolatedHeight);*/

	return interpolatedHeight;
}

void Mesh_Terrain::RefreshVertex() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vec4), &vertex[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}