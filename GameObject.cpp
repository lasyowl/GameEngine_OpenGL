#include "GameObject.h"


GameObject::GameObject() : camera(nullptr), player(nullptr), controller_character(nullptr), controller_static(nullptr),
render_terrain(nullptr), render_point(nullptr), render_sphere(nullptr), render_water(nullptr), render_particles(nullptr),
render_models_animated(nullptr), render_models_static(nullptr), render_button(nullptr), render_text(nullptr),
render_progressBar(nullptr), light(nullptr), framebuffer(nullptr), objFinder_editor(nullptr), gui(nullptr),
loader(nullptr), pickedObj(nullptr), editor_terrain(nullptr), postprocess(nullptr), texture_terrain(nullptr), terrain(nullptr) {

}

GameObject::~GameObject() {
	delete camera;
	delete player;
	delete controller_character;
	delete controller_static;
	delete render_terrain;
	delete render_point;
	delete render_postprocess;
	delete render_sphere;
	delete render_water;
	delete render_particles;
	delete render_models_animated;
	delete render_models_static;
	delete render_button;
	delete render_text;
	delete render_progressBar;
	delete terrain;
	delete texture_terrain;
	delete light;
	delete framebuffer;
	delete objFinder_editor;
	delete gui;
	delete loader;
	delete editor_terrain;
	delete postprocess;

	delete pickedObj;
}