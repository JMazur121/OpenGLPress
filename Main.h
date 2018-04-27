#include "Stage.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>

const unsigned int WIDTH = 1366;
const unsigned int HEIGHT = 768;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
static Camera* camera;

class Main
{
public:
	Main();
	int init();
	void processInput(GLFWwindow *window);
	void renderLoop();
	void prepareStage();
private:
	GLFWwindow* window;
	Stage stage;
};