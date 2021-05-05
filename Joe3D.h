#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>


struct Voxel {
	float x;
	float y;
	float z;
	float size;
	float rotation;
	void(*Update)(Voxel*);
};


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
		//Serilization functions
		void serilizeWriteVoxels(std::string file, std::vector<Voxel>* voxels) {
			std::fstream f;
			f.open(file, std::fstream::in | std::fstream::out | std::fstream::binary);
			if (!f) {
				std::fstream c;
				c.open(file, std::fstream::in | std::fstream::out | std::fstream::trunc);
				c.close();
				f.open(file, std::fstream::in | std::fstream::out | std::fstream::binary);
			}
			for (auto& i : (*voxels)) {
				f.write(reinterpret_cast<char*>(&i.x), sizeof(i.x));
				f.write("|", sizeof("|"));
				f.write(reinterpret_cast<char*>(&i.y), sizeof(i.y));
				f.write("|", sizeof("|"));
				f.write(reinterpret_cast<char*>(&i.z), sizeof(i.z));
				f.write("|", sizeof("|"));
			}

			f.close();
		}
		void serilizeReadVoxels(std::string file, std::vector<Voxel>* voxels) {
			std::fstream f;
			f.open(file, std::fstream::out | std::fstream::binary | std::fstream::app);
			if (!f) {
				return;
			}
			std::string line;
			Voxel v;
			while (std::getline(f, line)) {
				char* buf = new char[line.size()];
				f.read(buf, sizeof(line));
				std::cout << (unsigned int)buf << std::endl;
				delete(buf);
			}
			f.close();
		}
		//Will write
		void serilizeWrite(std::string file, std::fstream* f) {
			

		}
		void serilizeRead(std::string file, std::fstream* f) {
			
		}
	public:
		//Drawing functions
		void drawVoxel(GLfloat x, GLfloat y, GLfloat z, GLfloat size,GLfloat rotation) {
			glPushMatrix();
			glRotatef(rotation, 0.5, 0.5, 0.5);
			glBegin(GL_QUADS);
			glColor3f(0.5, 0.0, 0.0);
			//front
			glVertex3f(x-size, y, z);
			glVertex3f(x-size, y+size, z);
			glVertex3f(x, y+size, z);
			glVertex3f(x, y, z);
			//back
			glColor3f(0.0, 0.5, 0.0);
			glVertex3f(x, y, z+size);
			glVertex3f(x, y+size, z+size);
			glVertex3f(x-size, y+size, z+size);
			glVertex3f(x-size, y, z+size);
			//right
			glColor3f(0.0, 0.0, 0.5);
			glVertex3f(x, y, z);
			glVertex3f(x, y+size, z);
			glVertex3f(x, y+size, z+size);
			glVertex3f(x, y, z+size);
			//left
			glColor3f(0.5, 0.5, 0.0);
			glVertex3f(x-size, y, z+size);
			glVertex3f(x-size, y+size, z+size);
			glVertex3f(x-size, y+size, z);
			glVertex3f(x-size, y, z);
			//top
			glColor3f(0.0, 0.5, 0.5);
			glVertex3f(x-size, y, z+size);
			glVertex3f(x-size, y, z);
			glVertex3f(x, y, z);
			glVertex3f(x, y, z+size);
			//bottom
			glColor3f(0.5, 0.0, 0.5);
			glVertex3f(x-size, y+size, z+size);
			glVertex3f(x-size, y+size, z);
			glVertex3f(x, y+size, z);
			glVertex3f(x, y+size, z+size);
			glEnd();
			glPopMatrix();
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