#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <FreeImage/FreeImage.h>

#include "Model_Scene_Static.h"
#include "GlobalFunctions.h"

using namespace std;
using namespace glm;

Model_Scene_Static::Model_Scene_Static() {

}

Model_Scene_Static::~Model_Scene_Static() {
	glDeleteVertexArrays(vao.size(), &vao[0]);
	glDeleteBuffers(vbo_vertex.size(), &vbo_vertex[0]);
	glDeleteBuffers(vbo_normal.size(), &vbo_normal[0]);
	glDeleteBuffers(vbo_uv.size(), &vbo_uv[0]);

	glDeleteVertexArrays(1, &vao_selectionBox[0]);
	glDeleteBuffers(1, &vbo_selectionBox_vertex[0]);
}

int Model_Scene_Static::GetNumVao() {
	return vao.size();
}

int Model_Scene_Static::GetNumVertex(int index) {
	return numVertex[index];
}

void Model_Scene_Static::BindVao(int index) {
	glBindVertexArray(vao[index]);
}

void Model_Scene_Static::BindVao_SelectionBox(int index) {
	glBindVertexArray(vao_selectionBox[index]);
}

void Model_Scene_Static::LoadTexture(const char *filePath) {
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

void Model_Scene_Static::LoadScene(const FileInfo &fileInfo) {
	Assimp::Importer importer;
	importer.ReadFile(fileInfo.fullPath, aiProcess_Triangulate | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
	const aiScene *scene = importer.GetOrphanedScene();
	
	// Stage 1 : Mesh generation
	mesh.resize(scene->mNumMeshes);
	numVertex.resize(mesh.size());
	for (int i = 0; i < scene->mNumMeshes; i++) {
		mesh[i] = scene->mMeshes[i];
		numVertex[i] = mesh[i].vertex.size();
	}

	// Stage 2 : Node generation
	rootNode = scene->mRootNode;
	// And post generation
	//NodePostProcess(&rootNode);

	// Stage 3 : Material generation
	numMaterials = scene->mNumMaterials;
	materials.resize(numMaterials);
	for (int i = 0; i < numMaterials; i++) {
		aiString buffer;
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &buffer) == AI_SUCCESS) {
			materials[i].texture_diffuse = buffer.data;
			materials[i].useTexture = true;
			// **** Temporary patch for texture load ****
			if (materials[i].texture_diffuse[0] == '.')
				materials[i].texture_diffuse = materials[i].texture_diffuse.substr(3, materials[i].texture_diffuse.length() - 3);

			glGenTextures(1, &materials[i].tbo_diffuse);
			glActiveTexture(GL_TEXTURE0 + materials[i].tbo_diffuse);
			glBindTexture(GL_TEXTURE_2D, materials[i].tbo_diffuse);
			LoadTexture((fileInfo.filePath + materials[i].texture_diffuse).data());
			glGenerateMipmap(GL_TEXTURE_2D);

			//printf("%s\n", (fileInfo.filePath + materials[i].texture_diffuse).data());
			if (scene->mMaterials[i]->GetTexture(aiTextureType_HEIGHT, 0, &buffer) == AI_SUCCESS) {
				materials[i].texture_normal = buffer.data;
				materials[i].useTexture = true;
				// **** Temporary patch for texture load ****
				if (materials[i].texture_normal[0] == '.')
					materials[i].texture_normal = materials[i].texture_normal.substr(3, materials[i].texture_normal.length() - 3);
				
				glGenTextures(1, &materials[i].tbo_normal);
				glActiveTexture(GL_TEXTURE0 + materials[i].tbo_normal);
				glBindTexture(GL_TEXTURE_2D, materials[i].tbo_normal);
				LoadTexture((fileInfo.filePath + materials[i].texture_normal).data());
				glGenerateMipmap(GL_TEXTURE_2D);
				//printf("%s\n", (fileInfo.filePath + materials[i].texture_normal).data());
			}
			else {
				materials[i].tbo_normal = -1;
			}
		}
		aiColor4D ambient, diffuse, specular;
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_AMBIENT, &ambient);
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_SPECULAR, &specular);
		materials[i].CopyColor(ambient, COLOR_AMBIENT);
		materials[i].CopyColor(diffuse, COLOR_DIFFUSE);
		materials[i].CopyColor(specular, COLOR_SPECULAR);
	}

	// Stage 4 : Selection-box generation
	for (int i = 0; i < scene->mNumMeshes; i++) {
		selectionBox.x_max = std::max(selectionBox.x_max, mesh[i].selectionBox.x_max);
		selectionBox.x_min = std::min(selectionBox.x_min, mesh[i].selectionBox.x_min);
		selectionBox.y_max = std::max(selectionBox.y_max, mesh[i].selectionBox.y_max);
		selectionBox.y_min = std::min(selectionBox.y_min, mesh[i].selectionBox.y_min);
		selectionBox.z_max = std::max(selectionBox.z_max, mesh[i].selectionBox.z_max);
		selectionBox.z_min = std::min(selectionBox.z_min, mesh[i].selectionBox.z_min);
	}
	GenSelectionBoxVertice();

	GenObjects();
	BindObjectData();

	numMeshes = mesh.size();
}

void Model_Scene_Static::GenSelectionBoxVertice() {
	vertex_selectionBox.resize(16);
	vertex_selectionBox[0] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[1] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[2] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[3] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[4] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[5] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[6] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[7] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[8] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[9] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[10] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[11] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[12] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[13] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[14] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[15] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);

	selectionBox.vertices[0] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	selectionBox.vertices[1] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	selectionBox.vertices[2] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	selectionBox.vertices[3] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);
	selectionBox.vertices[4] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	selectionBox.vertices[5] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	selectionBox.vertices[6] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	selectionBox.vertices[7] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
}

void Model_Scene_Static::NodePostProcess(Model_Node *node) {
	for (int i = 0; i < node->numChild; i++) {
		NodePostProcess(&node->children[i]);
	}
}

void Model_Scene_Static::GenObjects() {
	vao.resize(mesh.size());
	vbo_vertex.resize(mesh.size());
	vbo_normal.resize(mesh.size());
	vbo_uv.resize(mesh.size());
	glGenVertexArrays(vao.size(), &vao[0]);
	glGenBuffers(vbo_vertex.size(), &vbo_vertex[0]);
	glGenBuffers(vbo_normal.size(), &vbo_normal[0]);
	glGenBuffers(vbo_uv.size(), &vbo_uv[0]);

	vao_selectionBox.resize(1);
	vbo_selectionBox_vertex.resize(1);
	glGenVertexArrays(1, &vao_selectionBox[0]);
	glGenBuffers(1, &vbo_selectionBox_vertex[0]);
}

void Model_Scene_Static::BindObjectData() {
	for (int i = 0; i < vao.size(); i++) {
		glBindVertexArray(vao[i]);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].vertex.size() * sizeof(vec3), &mesh[i].vertex[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].normal.size() * sizeof(vec3), &mesh[i].normal[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].uv.size() * sizeof(vec2), &mesh[i].uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}

	glBindVertexArray(vao_selectionBox[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_selectionBox_vertex[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex_selectionBox.size() * sizeof(vec3), &vertex_selectionBox[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
}