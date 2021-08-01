#include "Joe3D.h"

joe::Engine en;
GLfloat angle = 90.0f;
GLdouble cangle = 0.0;
float coold = 0;
float delta = 0;
Vec3d camera = { 0.0, 0.0, 0.0 };
double hcangle = 3.14;
double vcangle = 0.0;
double initalfov = 45.0;
double speed = 3.0;
double mspeed = 0.005;
std::vector<Voxel> voxels;
bool movecam = false;
GLFWwindow* wind;
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
//Convert this entire project into glfw
void movementUpdate(int key, int xx, int yy) {
	float fraction = 0.1f;
	switch (key) {
	case GLFW_KEY_LEFT:
		hcangle -= 0.01;
		break;
	case GLFW_KEY_RIGHT:
		hcangle += 0.01;
		break;
	case GLFW_KEY_UP:
		camera.z += 0.01;
		movecam = true;
		break;
	case GLFW_KEY_DOWN:
		camera.z -= 0.01;
		movecam = true;
		break;
	}
}
//Use rotate and translate on a final matrix to change entire screen
void mainRender(GLFWwindow* wind) {
	delta = en.getDelta();
	glfwSwapBuffers(wind);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	for (auto& i : voxels) {
		i.Update(&i);
	}
	glPopMatrix();
	glRotated(cangle, camera.x, camera.y, camera.z);
	if (movecam) {
		glTranslated(camera.x, camera.y, camera.z);
		movecam = false;
	}
	glPushMatrix();
	glfwSwapBuffers(wind);
	glFlush();
	glfwPollEvents();
}

//Convert everything to glfw and get to good position
int main(int argc, char* argv[]) {
	/*Voxel v = { 0.35f, 0.35f, 0.35f, 0.2f, 90.0f, "vUpdateRotating"};
	v.Update = &vUpdateRotating;
	Voxel v1 = { -0.75f, -0.45f, 0.3f, 0.1f, 90.0f, "vUpdate" };
	v1.Update = &vUpdate;
	voxels.push_back(v);
	voxels.push_back(v1);
	en.serilizeWriteVoxels("vox.vox", &voxels);*/
	en.serilizeReadVoxels("vox.vox", &voxels);
	en.init("Voxel Game", wind, 800, 800);
	wind = glfwCreateWindow(800, 800, "Test", NULL, NULL);
	glfwMakeContextCurrent(wind);
	glfwSwapInterval(1);
	for (auto& i : voxels) {
		if (i.findfunc) {
			setVoxelFunc(&i, i.funcname);
			i.findfunc = false;
		}
	}
	//glMatrixMode(GL_PROJECTION);
	while (!glfwWindowShouldClose(wind)) {
		mainRender(wind);

	}
	glfwDestroyWindow(wind);
	glfwTerminate();
	return 0;
}