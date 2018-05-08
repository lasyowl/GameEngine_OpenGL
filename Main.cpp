#define _SECURE_SCL 0
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//_CrtDumpMemoryLeaks();

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "GameManager.h"

static float VIEWPORT_X = 800.0f;
static float VIEWPORT_Y = 800.0f;

GameManager *gm;

void RegisterCallback() {
	glutKeyboardUpFunc(gm->keyboard.KeyboardUp);
	glutKeyboardFunc(gm->keyboard.KeyboardPress);
	glutDisplayFunc(gm->display.Draw);
	glutIdleFunc(gm->IdleFunc);
	glutMouseFunc(gm->mouse.MousePress);
	glutMotionFunc(gm->mouse.MouseMoveActive);
	glutPassiveMotionFunc(gm->mouse.MouseMovePassive);
	glutMouseWheelFunc(gm->mouse.MouseWheel);
	glutReshapeFunc(gm->display.Resize);
	glutSpecialUpFunc(gm->keyboard.SpecialKeyUp);
	glutSpecialFunc(gm->keyboard.SpecialKeyPress);
	glutMenuStatusFunc(gm->MenuStatusFunc);
}

int main(int argc, char **argv) {
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(VIEWPORT_X, VIEWPORT_Y);
	glutInitWindowPosition(800, 10);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Parse Test");
	glewExperimental = GL_TRUE;
	
	glewInit();
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	gm = new GameManager;
	gm->Initiate(VIEWPORT_X, VIEWPORT_Y);

	RegisterCallback();

	glutMainLoop();
	
	return 0;
}