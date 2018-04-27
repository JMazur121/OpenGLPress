#include <GL/glew.h>
#include <glm/vec3.hpp>

struct Uniforms {
	GLint color, position;
	GLint constant, linear, quadratic;
	GLint ambient, diffuse, specular;
};

struct Light
{
	glm::vec3 color;
	glm::vec3 position;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	GLfloat ambient;
	GLfloat diffuse;
	GLfloat specular;

	Uniforms uniforms;
	Light(GLfloat constant, GLfloat linear, GLfloat quadratic,
		glm::vec3 position, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		GLfloat ambient = 0.4f, GLfloat diffuse = 2.5f, GLfloat specular = 2.0f) :
		constant(constant), linear(linear), quadratic(quadratic), position(position), color(color), ambient(ambient), diffuse(diffuse), specular(specular) {}
};
