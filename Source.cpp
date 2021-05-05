#include "Joe3D.h"

joe::Engine en;
GLfloat angle = 90.0f;
float coold = 0;
float delta = 0;
std::vector<Voxel> voxels;
void vUpdateRotating(Voxel *p) {
	coold += delta;
	if (coold >= 0.01f) {
		coold = 0;
		(*p).rotation++;
	}
	//glTranslatef(0.0001f, 0.0001f, 0);
	en.drawVoxel((*p).x, (*p).y, (*p).z, (*p).size, (*p).rotation);
}
void vUpdate(Voxel *p) {
	en.drawVoxel((*p).x, (*p).y, (*p).z, (*p).size, (*p).rotation);
}

void mainRender() {
	delta = en.getDelta();
	//std::cout << delta << std::endl;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& i : voxels) {
		i.Update(&i);
	}
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	Voxel v = { 0.35f, 0.35f, 0.35f, 0.2f, 90.0f };
	v.Update = &vUpdateRotating;
	Voxel v1 = { -0.75f, -0.45f, 0.3f, 0.1f, 90.0f };
	v1.Update = &vUpdate;
	voxels.push_back(v);
	voxels.push_back(v1);
	en.serilizeWriteVoxels("vox.vox", &voxels);
	en.serilizeReadVoxels("vox.vox", &voxels);
	en.init(argc, argv, mainRender, 800, 800);
	
	return 0;
}