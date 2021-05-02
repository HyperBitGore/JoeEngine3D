#include "Joe3D.h"

joe::Engine en;
GLfloat angle = 90.0f;
float coold = 0;
void mainRender() {
	float delta = en.getDelta();
	std::cout << delta << std::endl;
	coold += delta;
	if (coold >= 0.01f) {
		angle++;
		coold = 0;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0.5f, 0.1f, 0.1f);
	en.drawVoxel(0.15, 0.15f, 0.15f, 0.1f, angle);
	//en.drawVoxel(0.1f, 0.1f, 0.25f, 5.0, 80.0f);
	//en.drawVoxel(0.1f, -0.2f, 0.25f, 5.0, 60.0f);
	glutSwapBuffers();
	//glFlush();
	//glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	en.init(argc, argv, mainRender, 800, 800);
	
	return 0;
}