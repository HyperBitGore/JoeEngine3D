#include "JoeEngine3D.h"


int main() {
	GLFWwindow* wind = Joe::Engine::initGL();
	
	//Joe::Files::LoadShaders("vertshader.glsl", "fragshader.glsl");
	GLuint programID = Joe::Files::LoadShaders("vertexshader.glsl", "fragmentshader.glsl");
	Joe::Files::loadBMP_Texture("xoK5F.bmp");
	std::vector<Joe::Model> models;
	Joe::Engine::addModel("monkey.obj", models);

	Joe::Controls control;
	GLuint lightmat = glGetUniformLocation(programID, "LIGHT");
	GLuint viewmatuniform = glGetUniformLocation(programID, "V");
	GLuint modlematuniform = glGetUniformLocation(programID, "M");
	GLuint matrixuniform = glGetUniformLocation(programID, "MVP");
	glm::vec3 lightpoint(0.5, 0.5, 1.5);
	double lastTime = 0;
	double mouserest = 0;
	while (glfwGetKey(wind, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(wind) == 0) {
		double currentTime = glfwGetTime();
		float delta = float(currentTime - lastTime);
		mouserest += delta;
		control.computeMatricesFromInputs(wind, delta);
		glm::mat4 proj = control.getProjectionMatrix();
		glm::mat4 viewm = control.getViewMatrix();
		glm::mat4 modm = glm::mat4(1.0);
		glm::mat4 MVP = proj * viewm * modm;
		glUseProgram(programID);
		glUniformMatrix4fv(matrixuniform, 1, GL_FALSE, &MVP[0][0]);
		glUniform3f(lightmat, lightpoint.x, lightpoint.y, lightpoint.z);
		glUniformMatrix4fv(viewmatuniform, 1, GL_FALSE, &viewm[0][0]);
		glUniformMatrix4fv(modlematuniform, 1, GL_FALSE, &modm[0][0]);
		Joe::Engine::drawWindow(wind, models);
		if (mouserest > 0.1) {
			glfwSetCursorPos(wind, 1024 / 2, 768 / 2);
			mouserest = 0;
		}
		lastTime = currentTime;
	}

	return 0;
}