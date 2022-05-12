#include "JoeEngine3D.h"


int main() {
	GLFWwindow* wind = Joe::Engine::initGL();
	
	Joe::Files::LoadShaders("vertshader.glsl", "fragshader.glsl");
	Joe::Files::loadBMP_Texture("xoK5F.bmp");
	while (glfwGetKey(wind, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(wind) == 0) {
		Joe::Engine::drawWindow(wind);
	}

	return 0;
}