#include "Mesh_Terrain.h"
#include "Editor_Terrain.h"

#include <GL/glew.h>
#include <FreeImage/FreeImage.h>

using namespace std;
using namespace glm;

Editor_Terrain::Editor_Terrain() : brushSize(100), currentBrush(0), textureMode(false), opacity(1.0f) {

}

Editor_Terrain::~Editor_Terrain() {

}

void Editor_Terrain::SetTargetMesh(Mesh_Terrain *mesh) {
	this->mesh = mesh;
}

void Editor_Terrain::SetTexture_Terrrain(Texture_Terrain *texture_terrain) {
	this->texture_terrain = texture_terrain;
}

void Editor_Terrain::SetViewport(vec2 *viewport) {
	this->viewport = viewport;
}

void Editor_Terrain::Initiate() {
	brush_height.resize(3);
	glGenTextures(1, &brush_height[0].tex_brush);
	glActiveTexture(GL_TEXTURE0 + brush_height[0].tex_brush);
	glBindTexture(GL_TEXTURE_2D, brush_height[0].tex_brush);
	LoadTextureToArray_Grayscale(&brush_height[0].texVal, "Res/Terrain/Height Brush/mountainRange/mountainRange_4.png");
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &brush_height[1].tex_brush);
	glActiveTexture(GL_TEXTURE0 + brush_height[1].tex_brush);
	glBindTexture(GL_TEXTURE_2D, brush_height[1].tex_brush);
	LoadTextureToArray_Grayscale(&brush_height[1].texVal, "Res/Terrain/Height Brush/Base.png");
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &brush_height[2].tex_brush);
	glActiveTexture(GL_TEXTURE0 + brush_height[2].tex_brush);
	glBindTexture(GL_TEXTURE_2D, brush_height[2].tex_brush);
	LoadTextureToArray_Grayscale(&brush_height[2].texVal, "Res/Terrain/Height Brush/noise/noiseLow.png");
	glGenerateMipmap(GL_TEXTURE_2D);

	LoadTextureToArray_Alphamap(&mesh->alphamap, texture_terrain->filePath_alphamap.data());
}

void Editor_Terrain::SetTexture(const int &texture) {
	this->currentTexture = texture;
}

void Editor_Terrain::SetBrush(const int &brush) {
	this->currentBrush = brush;
}

void Editor_Terrain::SetBrushSize(const float &brushSize) {
	this->brushSize = brushSize;
}

void Editor_Terrain::AdjustHeight(const vec3 &center, const bool &positive) {
	int index_LD = (int)-center.z * (mesh->width + 1) + (int)center.x;
	int brushWidth = brush_height[currentBrush].texVal.size();

	float invBrush = brushWidth / brushSize;
	int yOffset = -(int)brushWidth / 2 / invBrush;
	uvec2 center_xy(center.x, -center.z);

	for (int y = yOffset; y < -yOffset; y++) {
		for (int x = yOffset; x < -yOffset; x++) {
			uvec2 valid(center_xy.x + x, center_xy.y + y);
			if (valid.x < 0 || valid.x >= mesh->width || valid.y < 0 || valid.y >= mesh->width)
				continue;
			int index = index_LD + x + y * (mesh->width + 1);

			mesh->vertex[index].y += brush_height[currentBrush].texVal[y * invBrush + brushWidth / 2][x * invBrush + brushWidth / 2] * opacity * positive * 0.1f;
		}
	}
	
	mesh->RefreshVertex();
}

void Editor_Terrain::AdjustTexture(const vec3 &center) {
	int index_LD = (int)-center.z * mesh->width + (int)center.x;
	int brushWidth = brush_height[currentBrush].texVal.size();

	float invBrush = brushWidth / brushSize;
	int yOffset = -(int)brushWidth / 2 / invBrush;
	uvec2 center_xy(center.x, -center.z);

	for (int y = yOffset; y < -yOffset; y++) {
		for (int x = yOffset; x < -yOffset; x++) {
			uvec2 valid(center_xy.x + x, center_xy.y + y);
			if (valid.x < 0 || valid.x >= mesh->width || valid.y < 0 || valid.y >= mesh->width)
				continue;
			int index = index_LD + x + y * mesh->width;
			int currentBrushDensity = brush_height[currentBrush].texVal[y * invBrush + brushWidth / 2.0f][x * invBrush + brushWidth / 2.0f];
			int currentPixelAlpha = currentBrushDensity * opacity;
			GLubyte *textureA = &mesh->alphamap[index * 4];
			GLubyte *textureB = &mesh->alphamap[index * 4 + 1];
			GLubyte *textureC = &mesh->alphamap[index * 4 + 2];
			GLubyte *textureD = &mesh->alphamap[index * 4 + 3];

			if (currentPixelAlpha == 0)
				continue;

			if (currentTexture == 0) {
				*textureA = min(*textureA + currentPixelAlpha, 255);
				*textureB = max(*textureB - currentPixelAlpha, 0);
				*textureC = max(*textureC - currentPixelAlpha, 0);
				*textureD = max(*textureD - currentPixelAlpha, 0);
			}
			else if (currentTexture == 1) {
				*textureA = max(*textureA - currentPixelAlpha, 0);
				*textureB = min(*textureB + currentPixelAlpha, 255);
				*textureC = max(*textureC - currentPixelAlpha, 0);
				*textureD = max(*textureD - currentPixelAlpha, 0);
			}
			else if (currentTexture == 2) {
				*textureA = max(*textureA - currentPixelAlpha, 0);
				*textureB = max(*textureB - currentPixelAlpha, 0);
				*textureC = min(*textureC + currentPixelAlpha, 255);
				*textureD = max(*textureD - currentPixelAlpha, 0);
			}
			else if (currentTexture == 3) {
				*textureA = max(*textureA - currentPixelAlpha, 0);
				*textureB = max(*textureB - currentPixelAlpha, 0);
				*textureC = max(*textureC - currentPixelAlpha, 0);
				*textureD = min(*textureD + currentPixelAlpha, 255);
			}
			/*if (*textureA == currentTexture) {
				*mixFactor = max(*mixFactor - currentPixelAlpha, 0);
				if (*mixFactor == 0)
					*textureB = currentTexture;
			}
			else if (*textureB == currentTexture) {
				*mixFactor = min(*mixFactor + currentPixelAlpha, 255);
				if (*mixFactor == 255)
					*textureA = currentTexture;
			}
			else {
				if (*mixFactor > 127) {
					if (*mixFactor == 255) {
						*textureA = currentTexture;
					}
					else {
						*mixFactor = 255;
					}
				}
				else {
					if (*mixFactor == 0) {
						*textureB = currentTexture;
					}
					else {
						*mixFactor = 0;
					}
				}
			}*/
		}
	}

	texture_terrain->RefreshAlphamap();
}

void Editor_Terrain::LoadTextureToArray_Grayscale(vector<vector<int> > *target, const char *filePath) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	if (*filePath == NULL) {
		printf("NULL\n");
		return;
	}
	tx_file_format = FreeImage_GetFIFFromFilename(filePath);//FreeImage_GetFileType(filePath, 0);
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

	RGBQUAD color;

	vector<int> brush_line;
	brush_line.resize(width);
	target->resize(height);

	for (int i = 0; i < FreeImage_GetHeight(tx_pixmap_32); i++) {
		(*target)[i] = brush_line;
		for (int j = 0; j < FreeImage_GetWidth(tx_pixmap_32); j++) {
			FreeImage_GetPixelColor(tx_pixmap_32, j, i, &color);
			(*target)[i][j] = color.rgbRed;
		}
	}

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

void Editor_Terrain::LoadTextureToArray_Alphamap(vector<GLubyte> *target, const char *filePath) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	if (*filePath == NULL) {
		printf("NULL\n");
		return;
	}
	tx_file_format = FreeImage_GetFIFFromFilename(filePath);//FreeImage_GetFileType(filePath, 0);
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

	RGBQUAD color;
	
	target->resize(width * height * 4);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			FreeImage_GetPixelColor(tx_pixmap_32, j, i, &color);
			(*target)[4 * (i * width + j)] = color.rgbRed;
			(*target)[4 * (i * width + j) + 1] = color.rgbGreen;
			(*target)[4 * (i * width + j) + 2] = color.rgbBlue;
			(*target)[4 * (i * width + j) + 3] = color.rgbReserved;
		}
	}

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

void Editor_Terrain::SaveAlphamap(const char *filePath) {
	FIBITMAP *tx_pixmap_32 = FreeImage_Allocate(2048, 2048, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	if (tx_pixmap_32) {
		int bytespp;
		int y;
		int x;
		int width = FreeImage_GetWidth(tx_pixmap_32);
		int height = FreeImage_GetHeight(tx_pixmap_32);
		int line = FreeImage_GetLine(tx_pixmap_32);

		bytespp = line / width;
		for (y = 0; y < height; y++) {
			GLubyte *bits = FreeImage_GetScanLine(tx_pixmap_32, y);
			for (x = 0; x < width; x++) {
				int index = 4 * (width * y + x);
				bits[FI_RGBA_RED] = mesh->alphamap[index];
				bits[FI_RGBA_GREEN] = mesh->alphamap[index + 1];
				bits[FI_RGBA_BLUE] = mesh->alphamap[index + 2];
				bits[FI_RGBA_ALPHA] = mesh->alphamap[index + 3];
				// jump to next pixel
				bits += bytespp;
			}
		}

		FreeImage_Save(FIF_PNG, tx_pixmap_32, filePath, PNG_DEFAULT);

		FreeImage_Unload(tx_pixmap_32);
	}
}

void Editor_Terrain::SaveHeightmap(const char *filePath) {
	FIBITMAP *tx_pixmap_32 = FreeImage_Allocate(2049, 2049, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	if (tx_pixmap_32) {
		int bytespp;
		int y;
		int x;
		int width = FreeImage_GetWidth(tx_pixmap_32);
		int height = FreeImage_GetHeight(tx_pixmap_32);
		int line = FreeImage_GetLine(tx_pixmap_32);

		bytespp = line / width;
		for (y = 0; y < height; y++) {
			GLubyte *bits = FreeImage_GetScanLine(tx_pixmap_32, y);
			for (x = 0; x < width; x++) {
				int index = width * y + x;
				bits[FI_RGBA_RED] = mesh->vertex[index].g;
				bits[FI_RGBA_GREEN] = mesh->vertex[index].g;
				bits[FI_RGBA_BLUE] = mesh->vertex[index].g;
				bits[FI_RGBA_ALPHA] = 255;
				// jump to next pixel
				bits += bytespp;
			}
		}

		FreeImage_Save(FIF_PNG, tx_pixmap_32, filePath, PNG_DEFAULT);

		FreeImage_Unload(tx_pixmap_32);
	}
}