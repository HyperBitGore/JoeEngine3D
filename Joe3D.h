#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>

void idle() {

}
void changeViewPort(int w, int h) {
	glViewport(0, 0, w, h);
}

namespace joe {
	class Engine {
	private:
		std::chrono::high_resolution_clock::time_point old_d;
	public:
		//Use glTranslatef
		void drawVoxel(GLfloat x, GLfloat y, GLfloat z, GLint size,GLfloat rotation) {
			glRotatef(rotation, 0.5, 0.5, 0.5);
			glBegin(GL_QUADS);
			glColor3f(0.5, 0.0, 0.0);
			//front
			glVertex3f(-x, y, z);
			glVertex3f(-x, -y, z);
			glVertex3f(x, -y, z);
			glVertex3f(x, y, z);
			//back
			glColor3f(0.0, 0.5, 0.0);
			glVertex3f(x, y, -z);
			glVertex3f(x, -y, -z);
			glVertex3f(-x, -y, -z);
			glVertex3f(-x, y, -z);
			//right
			glColor3f(0.0, 0.0, 0.5);
			glVertex3f(x, y, z);
			glVertex3f(x, -y, z);
			glVertex3f(x, -y, -z);
			glVertex3f(x, y, -z);
			//left
			glColor3f(0.5, 0.5, 0.0);
			glVertex3f(-x, y, -z);
			glVertex3f(-x, -y, -z);
			glVertex3f(-x, -y, z);
			glVertex3f(-x, y, z);
			//top
			glColor3f(0.0, 0.5, 0.5);
			glVertex3f(-x, y, -z);
			glVertex3f(-x, y, z);
			glVertex3f(x, y, z);
			glVertex3f(x, y, -z);
			//bottom
			glColor3f(0.5, 0.0, 0.5);
			glVertex3f(-x, -y, -z);
			glVertex3f(-x, -y, z);
			glVertex3f(x, -y, z);
			glVertex3f(x, -y, -z);
			glEnd();

		}
	public:
		void init(int argc, char* argv[], void(*render)(), const int width, const int height){
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
			glutInitWindowSize(width, height);
			glutInitWindowPosition(50, 50);
			glutCreateWindow("Joe Engine");
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);
			glReadBuffer(GL_BACK);
			glMatrixMode(GL_MODELVIEW);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glutIdleFunc(idle);
			glutReshapeFunc(changeViewPort);
			glutDisplayFunc(render);
			GLenum err = glewInit();
			if (GLEW_OK != err) {
				printf("Error");
				return;
			}
			old_d = std::chrono::high_resolution_clock::now();
			glutMainLoop();
		}
		float getDelta() {
			std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> delta = std::chrono::duration_cast<std::chrono::duration<float>>(t - old_d);
			float dt = (float)delta.count();
			old_d = t;
			return dt;
		}
	};

}