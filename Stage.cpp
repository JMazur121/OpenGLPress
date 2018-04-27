#include "Stage.h"
#define PRESS_RADIUS 0.12f
#define PRESS_HEIGHT 1.0f
#define ELEMENTS_RADIUS 0.16f
#define PRESS_DELTA 0.005f
#define CONVEYOR_LENGTH 3.8f
#define CONVEYOR_WIDTH 0.5f
#define CONVEYOR_HEIGHT 0.05f
#define BELT_LENGTH 0.42f
#define BEGIN -2.65f //-2.65f
#define END 0.7f //0.7f
#define NBRICKS 6

Stage::Stage(Camera* cam, unsigned int width, unsigned int height)
{
	camera = cam;
	buildShaders(program_id,"vertexshader.vert","fragmentshader.frag");
	buildShaders(light_id, "lamp_ver_sh.vert", "lamp_fs_sh.frag");
	model = glGetUniformLocation(program_id, "model");
	view = glGetUniformLocation(program_id, "view");
	projection = glGetUniformLocation(program_id, "projection");
	texture = glGetUniformLocation(program_id, "texture");
	cameraPos = glGetUniformLocation(program_id, "viewPos");
	lampModel = glGetUniformLocation(light_id, "model");
	lampView = glGetUniformLocation(light_id, "view");
	lampProjection = glGetUniformLocation(light_id, "projection");
	lampColor = glGetUniformLocation(light_id, "color");
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST | GL_LIGHTING);
	//conveyor belt
	objects.push_back(Cube(CONVEYOR_WIDTH / 10, CONVEYOR_HEIGHT * 3, CONVEYOR_LENGTH + 0.05f, "tplate.jpg", glm::vec3(CONVEYOR_WIDTH / 2, 0.0f, -1.0f), false));
	objects.push_back(Cube(CONVEYOR_WIDTH / 10, CONVEYOR_HEIGHT * 3, CONVEYOR_LENGTH + 0.05f, objects[0].getTextureID(), glm::vec3(-CONVEYOR_WIDTH / 2, 0.0f, -1.0f), false));
	objects.push_back(Cube(CONVEYOR_WIDTH + 0.15f, CONVEYOR_HEIGHT + 0.2f, BELT_LENGTH, objects[0].getTextureID(), glm::vec3(0.0f, 0.0f, CONVEYOR_LENGTH / 2 - 1.0f + (BELT_LENGTH / 2)), false));
	objects.push_back(Cube(CONVEYOR_WIDTH + 0.15f, CONVEYOR_HEIGHT + 0.2f, BELT_LENGTH, objects[0].getTextureID(), glm::vec3(0.0f, 0.0f, -CONVEYOR_LENGTH / 2 - 1.0f - (BELT_LENGTH / 2)), false));
	//legs
	objects.push_back(Cube(0.08f, 0.8f, 0.08f, "steel.png", glm::vec3(CONVEYOR_WIDTH / 2 + 0.05f, 0.2f, -1.2f), false));
	objects.push_back(Cube(0.08f, 0.8f, 0.08f, objects[4].getTextureID(), glm::vec3(-CONVEYOR_WIDTH / 2 - 0.05f, 0.2f, -1.2f), false));
	objects.push_back(Cube(0.08f, 0.8f, 0.08f, objects[4].getTextureID(), glm::vec3(-CONVEYOR_WIDTH / 2 - 0.05f, 0.2f, -0.8f), false));
	objects.push_back(Cube(0.08f, 0.8f, 0.08f, objects[4].getTextureID(), glm::vec3(CONVEYOR_WIDTH / 2 + 0.05f, 0.2f, -0.8f), false));
	//cover
	objects.push_back(Cube(0.8f, 0.15f, 0.8f, objects[4].getTextureID(), glm::vec3(0.0f, 0.6f, -1.0f), false));
	//press element 0.9f
	objects.push_back(Cylinder(PRESS_HEIGHT, PRESS_RADIUS, "bronze.jpg", glm::vec3(0.0f, 0.9f, -1.0f)));
	presselemID = objects.size() - 1;
	pressDirection = Press_Movement::CONVEYOR_FORWARD;
	lastState = Press_Movement::CONVEYOR_FORWARD;
	//ground
	objects.push_back(Plane(100.0f, "floor.jpg", glm::vec3(0.0f, -0.05f, 0.0f), 10.0f, false));
	//overhead
	//objects.push_back(Plane(9.0f, "overhead.jpg", glm::vec3(0.0f, 3.0f, -1.0f), 1.0f, true));
	//terrain
	//objects.push_back(Cube(8.0f, 8.0f, 8.0f, "walls.jpg", glm::vec3(0.0f, 3.0f, -1.0f), true));

	bricks.push_back(Cylinder(0.15f, ELEMENTS_RADIUS, "brick.png", glm::vec3(0.0f, 0.075f + CONVEYOR_HEIGHT, 0.0f)));
	bricks.push_back(PressedCylinder(0.15f, PRESS_RADIUS, ELEMENTS_RADIUS, "brick.png", glm::vec3(0.0f, 0.075f + CONVEYOR_HEIGHT, 0.0f)));
	press_down = 0.9f - ((-PRESS_HEIGHT / 2 + 0.9f) - (CONVEYOR_HEIGHT + 0.15f - 0.15f*0.9f));
	caster = Lamp(0.05f, glm::vec3(0.0f, 2.0f, 0.0f), 7, 24);
	lights.push_back(Light(1.0f, 0.09f, 0.032f, glm::vec3(-0.4f, 1.0f, 0.0f)));//,glm::vec3(0.4f, 0.4f, 0.4f)
	lights.push_back(Light(1.0f, 0.09f, 0.032f, glm::vec3(0.5f, 1.0f, -2.0f)));//,glm::vec3(0.2f, 0.2f, 0.6f),
	lights.push_back(Light(1.0f, 0.09f, 0.032f, glm::vec3(0.4f, 1.0f, 0.0f)));//,glm::vec3(0.3f, 0.3f, 0.7f),
	initLightUniforms();
	initBricks();
	initBelt();
}

void Stage::addObject(DrawableObject object)
{
	objects.push_back(object);
}

void Stage::addLight(Light light)
{
	lights.push_back(light);
}

void Stage::drawStage()
{
	glClearColor(0.55, 0.64, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(program_id);

	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniform3fv(cameraPos, 1, glm::value_ptr(camera->getPosition()));

	if (pressDirection != STOP)
	{
		if (pressDirection == CONVEYOR_FORWARD)
			conveyorForward();
		else
			movePress();
	}

	for (size_t i = 0; i < lights.size(); ++i)
	{
		glUniform3fv(lights[i].uniforms.position, 1, glm::value_ptr(lights[i].position));
		glUniform3fv(lights[i].uniforms.color, 1, glm::value_ptr(lights[i].color));

		glUniform1f(lights[i].uniforms.constant, lights[i].constant);
		glUniform1f(lights[i].uniforms.linear, lights[i].linear);
		glUniform1f(lights[i].uniforms.quadratic, lights[i].quadratic);

		glUniform1f(lights[i].uniforms.ambient, lights[i].ambient);
		glUniform1f(lights[i].uniforms.diffuse, lights[i].diffuse);
		glUniform1f(lights[i].uniforms.specular, lights[i].specular);
		
	}

	for each (DrawableObject obj in objects)
	{
		drawObject(obj);
	}
	drawBricks();
	drawBelt();
	drawCasters();
}

void Stage::stop()
{
	if (pressDirection != STOP) {
		lastState = pressDirection;
		pressDirection = STOP;
	}
	else
		pressDirection = lastState;
}

void Stage::incrementSpeed()
{
	if (parts > 400)
		parts -= 5;
}

void Stage::decrementSpeed()
{
	parts += 5;
}

void Stage::drawObject(DrawableObject & object)
{
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(object.getModel()));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object.getTextureID());
	glUniform1i(texture, 0);
	object.draw();
}

std::string read_shader_code(const GLchar* shaderPath)
{
	std::ifstream shader_file;
	shader_file.exceptions(std::ifstream::badbit);

	shader_file.open(shaderPath);
	std::stringstream shader_stream;
	shader_stream << shader_file.rdbuf();
	shader_file.close();
	return shader_stream.str();
}

GLuint compile_shader(const GLchar* shaderCode, GLenum shaderType)
{
	GLuint shader_id = glCreateShader(shaderType);
	glShaderSource(shader_id, 1, &shaderCode, NULL);
	glCompileShader(shader_id);

	// Print compile errors if any
	GLint success = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader_id, sizeof(infoLog), NULL, infoLog);
		std::string msg = std::string("Shader compilation: ") + infoLog;
		throw std::exception(msg.c_str());
	}
	return shader_id;
}

void Stage::buildShaders(GLuint& id, std::string vs, std::string fs)
{
	// prepare vertex and fragment shaders
	std::string vertex_code = read_shader_code(vs.c_str());
	GLuint vertex_id = compile_shader(vertex_code.c_str(), GL_VERTEX_SHADER);

	std::string fragment_code = read_shader_code(fs.c_str());
	GLuint fragment_id = compile_shader(fragment_code.c_str(), GL_FRAGMENT_SHADER);

	// link shader program
	id = glCreateProgram();
	glAttachShader(id, vertex_id);
	glAttachShader(id, fragment_id);
	glLinkProgram(id);

	// Print linking errors if any
	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		std::string msg = std::string("Shader program linking: ") + infoLog;
		throw std::exception(msg.c_str());
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

void Stage::movePress()
{
	glm::vec3 pos = objects[presselemID].getPosition();
	if (pressDirection == UP)
	{
		if (pos.y < 0.9f)
			pos.y += PRESS_DELTA;
		else
		{
			pressDirection = CONVEYOR_FORWARD;
		}
	}
	else
	{
		if (pos.y > press_down)//0.625
			pos.y -= PRESS_DELTA;
		else
		{
			glm::vec3 pos = intactElements.front();
			intactElements.pop_front();
			pressedElements.emplace_back(pos);
			pressDirection = UP;
			o_move = fabs(BEGIN - END)/parts;
		}
	}
	objects[presselemID].setPosition(pos);
}

void Stage::conveyorForward()
{

	if (intactElements.front().z < -1.0f) //<
	{
		for (auto it = intactElements.begin(); it != intactElements.end(); ++it) {
			(*it).z += o_move;//+
		}
		for (auto it = pressedElements.begin(); it != pressedElements.end(); ++it) {
			(*it).z += o_move;//+
		}
		if (pressedElements.front().z >= END) //>=
			pressedElements.pop_front();
		if ((intactElements.back().z + o_delta) >= BEGIN) //>=
			intactElements.emplace_back(glm::vec3(0.0f, 0.125f, BEGIN));
	}
	else
	{
		pressDirection = DOWN;
	}
	if (pressDirection == DOWN)
		return;
	if (beltSegments.back().z <= belt_end) //<=
	{
		for (auto it = beltSegments.begin(); it != beltSegments.end(); ++it) {
			(*it).z += o_move;//+
		}
	}
	else
	{
		beltSegments.pop_front();
		beltSegments.emplace_back(glm::vec3(0.0f, CONVEYOR_HEIGHT, belt_spawn));
	}
}

void Stage::drawBricks()
{
	for each (glm::vec3 pos in pressedElements)
	{
		bricks[1].setPosition(pos);
		drawObject(bricks[1]);
	}
	for each (glm::vec3 pos in intactElements)
	{
		bricks[0].setPosition(pos);
		drawObject(bricks[0]);
	}
}

void Stage::initBricks()
{
	float dst = (fabs(BEGIN - END)) / (NBRICKS);
	float speed;
	speed = fabs(BEGIN - END)/parts;
	o_move = speed;
	int i = 0;
	if (END > BEGIN) //move to +z
	{
		o_delta = -dst;
		do
		{
			intactElements.push_front(glm::vec3(0.0f, 0.125f, BEGIN + (i*dst)));
			++i;
		} while ((BEGIN + (i*dst)) <= -1.0f);

		while ((BEGIN + (i*dst)) < END)
		{
			pressedElements.push_front(glm::vec3(0.0f, 0.125f, BEGIN + (i*dst)));
			++i;
		}
	}
	else
	{
		o_delta = dst;
		do
		{
			intactElements.push_front(glm::vec3(0.0f, 0.125f, BEGIN - (i*dst)));
			++i;
		} while ((BEGIN - (i*dst)) >= -1.0f);

		while ((BEGIN - (i*dst)) > END)
		{
			pressedElements.push_front(glm::vec3(0.0f, 0.125f, BEGIN - (i*dst)));
			++i;
		}
	}
}

void Stage::initBelt()
{
	int nsegments;
	float z;
	nsegments = (int)ceil(CONVEYOR_LENGTH / BELT_LENGTH);
	belt_length = CONVEYOR_LENGTH / nsegments;
	beltFragment = Plane(CONVEYOR_WIDTH, belt_length, "rubber2.jpg", glm::vec3(0.0f, CONVEYOR_HEIGHT, 0.0f), 1.0f, false);
	if (END > BEGIN) //move to +z
	{
		z = -CONVEYOR_LENGTH / 2 - 1.0f - belt_length / 2;
		belt_spawn = z;
		belt_end = -CONVEYOR_LENGTH / 2 - 1.0f + belt_length / 2;
		beltSegments.emplace_front(glm::vec3(0.0f, CONVEYOR_HEIGHT, z));
		z += belt_length;
		for (int i = 0; i < nsegments; ++i)
		{
			beltSegments.emplace_front(glm::vec3(0.0f, CONVEYOR_HEIGHT, z));
			z += belt_length;
		}
	}
	else
	{
		z = CONVEYOR_LENGTH / 2 - 1.0f + belt_length / 2;
		belt_spawn = z;
		belt_end = CONVEYOR_LENGTH / 2 - 1.0f - belt_length / 2;
		beltSegments.emplace_front(glm::vec3(0.0f, CONVEYOR_HEIGHT, z));
		z -= belt_length;
		for (int i = 0; i < nsegments; ++i)
		{
			beltSegments.emplace_front(glm::vec3(0.0f, CONVEYOR_HEIGHT, z));
			z -= belt_length;
		}
	}
}

void Stage::drawBelt()
{
	for each (glm::vec3 pos in beltSegments)
	{
		beltFragment.setPosition(pos);
		drawObject(beltFragment);
	}
}

void Stage::drawCasters()
{
	glUseProgram(light_id);
	glUniformMatrix4fv(lampView, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(lampProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	for (size_t i = 0; i < lights.size(); ++i)
	{
		caster.setPosition(lights[i].position);
		glUniformMatrix4fv(lampModel, 1, GL_FALSE, glm::value_ptr(caster.getModel()));
		glUniform3fv(lampColor, 1, glm::value_ptr(lights[i].color));
		caster.draw();
	}
}

void Stage::initLightUniforms()
{
	char uniform[100];
	for (size_t i = 0; i < lights.size(); ++i)
	{
		sprintf(uniform, "lights[%d].color", i);
		lights[i].uniforms.color = glGetUniformLocation(program_id, uniform);
		sprintf(uniform, "lights[%d].position", i);
		lights[i].uniforms.position = glGetUniformLocation(program_id, uniform);

		sprintf(uniform, "lights[%d].constant", i);
		lights[i].uniforms.constant = glGetUniformLocation(program_id, uniform);
		sprintf(uniform, "lights[%d].quadratic", i);
		lights[i].uniforms.quadratic = glGetUniformLocation(program_id, uniform);
		sprintf(uniform, "lights[%d].linear", i);
		lights[i].uniforms.linear = glGetUniformLocation(program_id, uniform);

		sprintf(uniform, "lights[%d].ambient", i);
		lights[i].uniforms.ambient = glGetUniformLocation(program_id, uniform);
		sprintf(uniform, "lights[%d].diffuse", i);
		lights[i].uniforms.diffuse = glGetUniformLocation(program_id, uniform);
		sprintf(uniform, "lights[%d].specular", i);
		lights[i].uniforms.specular = glGetUniformLocation(program_id, uniform);
	}
}
