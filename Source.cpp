#include "Joe3D.h"

joe::Engine en;
GLfloat angle = 90.0f;
GLdouble cangle = 0.0;
float coold = 0;
float delta = 0;
Vec3d camera = { 0.0, 1.0, 0.0 };
double lx = 0.0;
double lz = 0.0;
std::vector<Voxel> voxels;
void vUpdateRotating(Voxel *p) {
	glPopMatrix();
	coold += delta;
	if (coold >= 0.01f) {
		coold = 0;
		(*p).rotation++;
	}
	//glTranslatef(0.0001f, 0.0001f, 0);
	en.drawVoxel((*p).x, (*p).y, (*p).z, (*p).size, (*p).rotation);
	glPushMatrix();
}
void vUpdate(Voxel *p) {
	//joe::Engine en;
	glPopMatrix();
	en.drawVoxel((*p).x, (*p).y, (*p).z, (*p).size, (*p).rotation);
	glPushMatrix();
}
//This doesnt rly work
void movementUpdate(int key, int xx, int yy) {
	float fraction = 0.1f;
	switch (key) {
	case GLUT_KEY_LEFT:
		cangle -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		cangle += 0.01;
		break;
	case GLUT_KEY_UP:
		camera.x += 0.01;
		camera.z += 0.01;
		break;
	case GLUT_KEY_DOWN:
		camera.x -= 0.01;
		camera.z -= 0.01;
		break;
	}
}

void mainRender() {
	delta = en.getDelta();
	//std::cout << delta << std::endl;
	glutSwapBuffers();
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	for (auto& i : voxels) {
		if (i.findfunc) {
			setVoxelFunc(&i, i.funcname);
			i.findfunc = false;
		}
		i.Update(&i);
	}
	glPopMatrix();
	glRotated(cangle, camera.x, camera.y, camera.z);
	glPushMatrix();
	//glTranslated(-camera.x, -camera.y, -camera.z);
	glFlush();
	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	/*Voxel v = { 0.35f, 0.35f, 0.35f, 0.2f, 90.0f, "vUpdateRotating"};
	v.Update = &vUpdateRotating;
	Voxel v1 = { -0.75f, -0.45f, 0.3f, 0.1f, 90.0f, "vUpdate" };
	v1.Update = &vUpdate;
	voxels.push_back(v);
	voxels.push_back(v1);
	en.serilizeWriteVoxels("vox.vox", &voxels);*/
	en.serilizeReadVoxels("vox.vox", &voxels);

	en.init(argc, argv, mainRender, movementUpdate, 800, 800);
	
	return 0;
}