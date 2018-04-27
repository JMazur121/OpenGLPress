#include "DrawableObject.h"
#include "Light.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

enum Press_Movement {
	UP,
	DOWN,
	CONVEYOR_FORWARD,
	STOP
};

class Stage
{
public:
	Stage() {}
	Stage(Camera* cam, unsigned int width, unsigned int height);
	void addObject(DrawableObject object);
	void addLight(Light light);
	void drawStage();
	void stop();
	void incrementSpeed();
	void decrementSpeed();
private:
	std::vector<DrawableObject> objects;
	std::vector<DrawableObject> bricks;
	std::vector<Light> lights;
	DrawableObject beltFragment;
	DrawableObject caster;
	int presselemID;
	Press_Movement pressDirection;
	Press_Movement lastState;
	std::list<glm::vec3> pressedElements;
	std::list<glm::vec3> intactElements;
	std::list<glm::vec3> beltSegments;
	GLint model, view, projection;
	GLint texture, cameraPos;
	GLint lampModel, lampView, lampProjection, lampColor;
	GLuint program_id,light_id;
	Camera* camera;
	glm::mat4 projectionMatrix;
	float o_delta = 0.0f;
	float o_move = 0.0f;
	float belt_length, belt_end, belt_spawn, press_down;
	int parts = 1000;

	void drawObject(DrawableObject& object);
	void buildShaders(GLuint& id,std::string vs, std::string fs);
	void movePress();
	void conveyorForward();
	void drawBricks();
	void initBricks();
	void initBelt();
	void drawBelt();
	void drawCasters();
	void initLightUniforms();
};