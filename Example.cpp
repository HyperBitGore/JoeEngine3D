#include "JoeEngine3D.h"

//https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
int main() {
	GLFWwindow* wind = Joe::Engine::initGL(1024, 768);
	
	//Joe::Files::LoadShaders("vertshader.glsl", "fragshader.glsl");
	GLuint programID = Joe::Files::LoadShaders("vertshader.glsl", "fragshader.glsl");
	GLuint tex1 = Joe::Files::loadBMP_Texture("xoK5F.bmp");
	GLuint tex2 = Joe::Files::loadBMP_Texture("tex2.bmp");
	//models/entities
	std::vector<Joe::Model> models;
	Joe::Engine::addModel("monkey.obj", models, tex1);
	Joe::Engine::addModel("monkey.obj", models, tex2);
	Joe::Engine::moveModelVertices(&models[1], glm::vec3(1.0, 0.0, 0.0));
	Joe::Entity m1 = Joe::Engine::createEntity(&models[0]);
	Joe::Entity m2 = Joe::Engine::createEntity(&models[1]);

	std::vector<Joe::Entity*> entities;
	entities.push_back(&m1);
	entities.push_back(&m2);

	Joe::Ray ray1 = { glm::vec3(4.0, 0.5, 0.5), glm::vec3(-0.1, 0.0, 0.0), glm::vec3(0) };

	//variables needed for main loop
	Joe::Controls control;
	GLuint lightmat = glGetUniformLocation(programID, "LIGHT");
	GLuint viewmatuniform = glGetUniformLocation(programID, "V");
	GLuint modlematuniform = glGetUniformLocation(programID, "M");
	GLuint matrixuniform = glGetUniformLocation(programID, "MVP");
	glm::vec3 lightpoint(0.5, 0.5, 1.5);
	double lastTime = 0;
	double mouserest = 0;
	double monkemove = 0;
	double raymove = 0;
	//actual main loop
	while (glfwGetKey(wind, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(wind) == 0) {
		double currentTime = glfwGetTime();
		float delta = float(currentTime - lastTime);
		mouserest += delta;
		monkemove += delta;
		raymove += delta;
		control.computeMatricesFromInputs(wind, delta);
		glm::mat4 proj = control.getProjectionMatrix();
		glm::mat4 viewm = control.getViewMatrix();
		//model matrix
		glm::mat4 modm = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
		glm::mat4 MVP = proj * viewm * modm;
		glUseProgram(programID);
		glUniformMatrix4fv(matrixuniform, 1, GL_FALSE, &MVP[0][0]);
		glUniform3f(lightmat, lightpoint.x, lightpoint.y, lightpoint.z);
		glUniformMatrix4fv(viewmatuniform, 1, GL_FALSE, &viewm[0][0]);
		glUniformMatrix4fv(modlematuniform, 1, GL_FALSE, &modm[0][0]);
		Joe::Engine::drawWindow(wind, entities);
		if (mouserest > 0.1) {
			glfwSetCursorPos(wind, 1024 / 2, 768 / 2);
			mouserest = 0;
		}
		//moves entity over
		if (monkemove > 0.1) {
			Joe::Engine::moveEntityVertices(&m2, glm::vec3(0.01, 0.0, 0.0));
			Joe::Engine::moveAABB(&m2.bounding, glm::vec3(0.01, 0.0, 0.0));
			if (Joe::Engine::AABBcollision(m1.bounding, m2.bounding)) {
				std::cout << "Colliding!" << "\n";
			}
			monkemove = 0;
		}
		if (raymove > 0.05) {
			Joe::Ray og = ray1;
			if (Joe::Engine::castRay(&ray1, entities, glm::vec3(5.0))) {
				std::cout << ray1.point.x << "\n";
			}
			ray1 = og;
			raymove = 0;
		}
		lastTime = currentTime;
	}

	return 0;
}