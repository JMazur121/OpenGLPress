#include "Main.h"
#include <windows.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

Main::Main() {}

int Main::init()
{
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "GLFW initialization failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		window = glfwCreateWindow(WIDTH, HEIGHT, "Press", nullptr, nullptr);
		if (window == nullptr) {
			throw std::exception("GLFW window not created");
		}
		glfwMakeContextCurrent(window);
		glfwSetCursorPosCallback(window,mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, lastX, lastY);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw std::exception("GLEW Initialization failed");
		glViewport(0, 0, WIDTH, HEIGHT);
		camera = new Camera(glm::vec3(1.0f, 1.0f, 3.0f));
		return 0;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		glfwTerminate();
		return -1;
	}
}

void Main::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->stage.stop();
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		this->stage.incrementSpeed();
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		this->stage.decrementSpeed();
}

void Main::renderLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		processInput(window);
		stage.drawStage();
		glfwSwapBuffers(window);
		//Sleep(30);
	}
	glfwTerminate();
}

void Main::prepareStage()
{
	try
	{
		stage = Stage(camera, WIDTH, HEIGHT);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Stage creating error" << std::endl;
		std::cout << ex.what() << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
}

int main()
{
	Main sceneObject;
	if ((sceneObject.init()) < 0)
	{
		std::cout << "Something went wrong" << std::endl;
		return -1;
	}
	sceneObject.prepareStage();
	sceneObject.renderLoop();
	delete camera;
	return 0;
}