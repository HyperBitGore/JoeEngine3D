#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
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
	std::string funcname;
	bool findfunc;
	void(*Update)(Voxel*);
};
typedef void (*UpdateFunc)(Voxel*);


void vUpdate(Voxel *p);
void vUpdateRotating(Voxel *p);


std::map<std::string, void(*)(Voxel*)> voxfmap;
//Fix the key for some reason not being found
void setVoxelFunc(Voxel *p, std::string f) {
	std::map<std::string, void(*)(Voxel*)>::iterator id;
	id = voxfmap.find(f);
	if (id != voxfmap.end()) {
		(*p).Update = voxfmap.at(f);
	}
	
}

void idle() {}
void changeViewPort(int w, int h) {
	glViewport(0, 0, w, h);
}

namespace joe {
	class Engine {
	private:
		std::chrono::high_resolution_clock::time_point old_d;
	public:
		//Serilization functions
		//Rewrite this to not use binary and use human readable format you nonce
		void serilizeWriteVoxels(std::string file, std::vector<Voxel>* voxels) {
			std::fstream f;
			f.open(file, std::fstream::in | std::fstream::out);
			if (!f) {
				std::fstream c;
				c.open(file, std::fstream::in | std::fstream::out | std::fstream::trunc);
				c.close();
				f.open(file, std::fstream::in | std::fstream::out);
			}
			for (auto& i : (*voxels)) {
				f << std::to_string(i.x) + "|";
				f << std::to_string(i.y) + "|";
				f << std::to_string(i.z) + "|";
				f << std::to_string(i.size) + "|";
				f << std::to_string(i.rotation) + "|";
				f << i.funcname + "|";
				f << "[]\n";
			}

			f.close();
		}
		void serilizeReadVoxels(std::string file, std::vector<Voxel>* voxels) {
			std::fstream f;
			f.open(file, std::fstream::in | std::fstream::out | std::fstream::app);
			if (!f) {
				std::cout << "Error opening vox file" << std::endl;
				return;
			}
			std::string line;
			int mode = 1;
			while (std::getline(f, line)) {
				Voxel v;
				std::string n;
				mode = 1;
				for (int i = 0; i < line.size(); i++) {
					if (line[i] == '|') {
						switch (mode) {
						case 1:
							v.x = std::stof(n);
							std::cout << v.x << "|";
							break;
						case 2:
							v.y = std::stof(n);
							std::cout << v.y << "|";
							break;
						case 3:
							v.z = std::stof(n);
							std::cout << v.z << "|";
							break;
						case 4:
							v.size = std::stof(n);
							std::cout << v.size << "|";
							break;
						case 5:
							v.rotation = std::stof(n);
							std::cout << v.rotation << "|";
							break;
						case 6:
							v.funcname = n;
							v.findfunc = true;
							std::cout << " " + v.funcname + " " << std::endl;
							voxels->push_back(v);
							break;
						}
						mode++;
						n.clear();
					}
					else {
						if (line[i] != '\n') {
							n.push_back(line[i]);
						}
					}

				}
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
			std::string l = "vUpdate";
			voxfmap.insert(std::pair<std::string, void(*)(Voxel*)>(l, &vUpdate));
			l = "vUpdateRotating";
			voxfmap.insert(std::pair<std::string, void(*)(Voxel*)>(l, &vUpdateRotating));
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
void vUpdate(Voxel *p) {
	joe::Engine en;
	en.drawVoxel((*p).x, (*p).y, (*p).z, (*p).size, (*p).rotation);
}