#include "Render_Collider.h"
#include "Render_Sphere.h"

using namespace std;
using namespace glm;

Render_Collider::Render_Collider() : render_sphere(nullptr), currentRenderer(RENDERER_SPHERE) {
	
}

Render_Collider::~Render_Collider() {

}

void Render_Collider::SetRenderer(Render *renderer, const int &rendererEnum) {
	switch (rendererEnum) {
	case RENDERER_SPHERE:
		render_sphere = (Render_Sphere*)renderer;
		break;
	default:
		break;
	}
}

void Render_Collider::Initiate() {
	GenSphereObject();
}

void Render_Collider::SetVariables(const int &variableEnum, const float &value) {
	switch (variableEnum) {
	case VARIABLE_RADIUS:
		render_sphere->SetRadius(value);
		break;
	default:
		break;
	}
}

void Render_Collider::SelectRenderer(const int &rendererEnum) {
	currentRenderer = rendererEnum;
}

void Render_Collider::Draw(const mat4 &ViewProjectionMatrix) {
	render_sphere->Draw(ViewProjectionMatrix, vao[RENDERER_SPHERE], mesh_sphere.index.size());
}

void Render_Collider::GenSphereObject() {
	mesh_sphere.InitMesh(10);
	glGenVertexArrays(1, &vao[RENDERER_SPHERE]);
	glGenBuffers(1, &vbo_vertex[RENDERER_SPHERE]);
	glGenBuffers(1, &index[RENDERER_SPHERE]);
	
	glBindVertexArray(vao[RENDERER_SPHERE]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[RENDERER_SPHERE]);
	glBufferData(GL_ARRAY_BUFFER, mesh_sphere.vertex.size() * 3 * sizeof(float), &mesh_sphere.vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index[RENDERER_SPHERE]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_sphere.index.size() * sizeof(GLuint), &mesh_sphere.index[0], GL_STATIC_DRAW);
}