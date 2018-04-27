#include "Texture.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class DrawableObject
{
public:
	DrawableObject() {};
	void draw();
	void setPosition(glm::vec3 pos);
	void setModel(glm::mat4 model);
	glm::vec3 getPosition();
	glm::mat4 getModel();
	GLuint getTextureID();
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;
	Texture texture;
	glm::vec3 position;
	glm::mat4 model;
	void calculateModel();
	void configureBuffers();
};

class Plane : public DrawableObject {
public:
	Plane(float size, std::string filePath, glm::vec3 pos, float textureMultiplier, bool inverseNormals);
	Plane(float x, float z, std::string filePath, glm::vec3 pos, float textureMultiplier, bool inverseNormals);
};

class Cube : public DrawableObject {
public:
	Cube(float xScale, float yScale, float zScale, bool inverseNormals);
	Cube(float xScale, float yScale, float zScale, std::string filePath, bool inverseNormals);
	Cube(float xScale, float yScale, float zScale, std::string filePath, glm::vec3 pos, bool inverseNormals);
	Cube(float xScale, float yScale, float zScale, GLuint texID, glm::vec3 pos, bool inverseNormals);
};

class Cylinder : public DrawableObject {
public:
	Cylinder(float height, float radius);
	Cylinder(float height, float radius, std::string filePath, glm::vec3 pos);
	Cylinder(float height, float radius, GLuint texID, glm::vec3 pos);
	static void generateSides(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, float height,float yTranslation, bool inverseNormal);
	static void generateBase(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, float height, bool normalUp);
};

class PressedCylinder : public DrawableObject {
public:
	PressedCylinder(float height, float innerRadius, float outerRadius, float ratio = 0.5f);
	PressedCylinder(float height, float innerRadius, float outerRadius, std::string filePath, glm::vec3 pos, float ratio = 0.5f);
	PressedCylinder(float height, float innerRadius, float outerRadius, GLuint texID, glm::vec3 pos, float ratio = 0.5f);
};

class Lamp : public DrawableObject {
public:
	Lamp(float xScale, float yScale, float zScale, glm::vec3 pos);
	Lamp(float radius, glm::vec3 pos,int latitude = 2, int longitude = 3);
};