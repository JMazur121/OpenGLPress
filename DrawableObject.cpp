#include "DrawableObject.h"
#define PI 3.14159265
#define CYLINDER_SIDES 100


static GLfloat unit_cube[] = {
	//positions			//texture coordinates	//normals
	//x      y      z
	//front
	-0.5f ,-0.5f , 0.5f,	0.0f, 0.0f,		0.0f,0.0f,1.0f,
	0.5f ,-0.5f , 0.5f,		0.0f, 1.0f,		0.0f,0.0f,1.0f,
	-0.5f , 0.5f , 0.5f,	1.0f, 0.0f,		0.0f,0.0f,1.0f,
	0.5f , 0.5f , 0.5f,		1.0f, 1.0f,		0.0f,0.0f,1.0f,

	//back
	-0.5f , 0.5f ,-0.5f,	1.0f, 1.0f,		0.0f,0.0f,-1.0f,
	0.5f , 0.5f ,-0.5f,		0.0f, 1.0f,		0.0f,0.0f,-1.0f,
	-0.5f ,-0.5f ,-0.5f,	1.0f, 0.0f,		0.0f,0.0f,-1.0f,
	0.5f ,-0.5f ,-0.5f,		0.0f, 0.0f,		0.0f,0.0f,-1.0f,

	//up
	-0.5f , 0.5f , 0.5f,	0.0f, 0.0f,		0.0f,1.0f,0.0f,
	0.5f , 0.5f , 0.5f,		1.0f, 0.0f,		0.0f,1.0f,0.0f,
	-0.5f , 0.5f ,-0.5f,	0.0f, 1.0f,		0.0f,1.0f,0.0f,
	0.5f , 0.5f ,-0.5f,		1.0f, 1.0f,		0.0f,1.0f,0.0f,

	//bottom
	-0.5f ,-0.5f ,-0.5f,	1.0f, 1.0f,		0.0f,-1.0f,0.0f,
	0.5f ,-0.5f ,-0.5f,		1.0f, 0.0f,		0.0f,-1.0f,0.0f,
	-0.5f ,-0.5f , 0.5f,	0.0f, 1.0f,		0.0f,-1.0f,0.0f,
	0.5f ,-0.5f , 0.5f,		0.0f, 0.0f,		0.0f,-1.0f,0.0f,

	//right
	0.5f ,-0.5f , 0.5f,		0.0f, 0.0f,		1.0f,0.0f,0.0f,
	0.5f ,-0.5f ,-0.5f,		0.0f, 1.0f,		1.0f,0.0f,0.0f,
	0.5f , 0.5f , 0.5f,		1.0f, 0.0f,		1.0f,0.0f,0.0f,
	0.5f , 0.5f ,-0.5f,		1.0f, 1.0f,		1.0f,0.0f,0.0f,

	//left
	-0.5f ,-0.5f ,-0.5f,	0.0f, 0.0f,		-1.0f,0.0f,0.0f,
	-0.5f ,-0.5f , 0.5f,	1.0f, 0.0f,		-1.0f,0.0f,0.0f,
	-0.5f , 0.5f ,-0.5f,	0.0f, 1.0f,		-1.0f,0.0f,0.0f,
	-0.5f , 0.5f , 0.5f,	1.0f, 1.0f,		-1.0f,0.0f,0.0f
};

static GLuint unit_cube_indices[] ={ 
	0,  1,  2,
	2,  1,  3,
	4,  5,  6,
	6,  5,  7,
	8,  9, 10,
	10,  9, 11,
	12, 13, 14,
	14, 13, 15,
	16, 17, 18,
	18, 17, 19,
	20, 21, 22,
	22, 21, 23, 
};

static GLfloat plane_vertices[] = {
	// positions          // texture coords		//normals
	0.5f,  0.0f, 0.5f,   1.0f, 1.0f,		0.0f,1.0f,0.0f,
	0.5f,  0.0f, -0.5f,   1.0f, 0.0f,		0.0f,1.0f,0.0f,
	-0.5f, 0.0f, -0.5f,   0.0f, 0.0f,		0.0f,1.0f,0.0f,
	-0.5f,  0.0f, 0.5f,   0.0f, 1.0f,		0.0f,1.0f,0.0f
};

static GLuint plane_indices[] = {
	0, 1, 3,
	1, 2, 3
};

void DrawableObject::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void DrawableObject::setPosition(glm::vec3 pos)
{
	position = pos;
	calculateModel();
}

void DrawableObject::setModel(glm::mat4 model)
{
	this->model = model;
}

glm::vec3 DrawableObject::getPosition()
{
	return position;
}

glm::mat4 DrawableObject::getModel()
{
	return model;
}

GLuint DrawableObject::getTextureID()
{
	return texture.getID();
}

void DrawableObject::calculateModel()
{
	glm::mat4 trans;
	trans = glm::translate(trans, position);
	model = trans;
}

void DrawableObject::configureBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);//vertex position

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//texture coordinates

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//normals

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Plane::Plane(float size, std::string filePath, glm::vec3 pos, float textureMultiplier, bool inverseNormals)
{
	texture = Texture(filePath);
	float inv = (inverseNormals) ? -1.0f : 1.0f;
	for (size_t i = 0; i < 4; ++i)
	{
		//x,y,z
		vertices.push_back(plane_vertices[i * 8] * size);
		vertices.push_back(plane_vertices[i * 8 + 1]);
		vertices.push_back(plane_vertices[i * 8 + 2] * size);
		//texture
		vertices.push_back(plane_vertices[i * 8 + 3]*textureMultiplier);
		vertices.push_back(plane_vertices[i * 8 + 4]*textureMultiplier);
		//normals
		vertices.push_back(plane_vertices[i * 8 + 5] * inv);
		vertices.push_back(plane_vertices[i * 8 + 6] * inv);
		vertices.push_back(plane_vertices[i * 8 + 7] * inv);
	}

	for each (GLuint var in plane_indices)
	{
		indices.push_back(var);
	}
	configureBuffers();
	setPosition(pos);
}

Plane::Plane(float x, float z, std::string filePath, glm::vec3 pos, float textureMultiplier, bool inverseNormals)
{
	texture = Texture(filePath);
	float inv = (inverseNormals) ? -1.0f : 1.0f;
	for (size_t i = 0; i < 4; ++i)
	{
		//x,y,z
		vertices.push_back(plane_vertices[i * 8] * x);
		vertices.push_back(plane_vertices[i * 8 + 1]);
		vertices.push_back(plane_vertices[i * 8 + 2] * z);
		//texture
		vertices.push_back(plane_vertices[i * 8 + 3] * textureMultiplier);
		vertices.push_back(plane_vertices[i * 8 + 4] * textureMultiplier);
		//normals
		vertices.push_back(plane_vertices[i * 8 + 5] * inv);
		vertices.push_back(plane_vertices[i * 8 + 6] * inv);
		vertices.push_back(plane_vertices[i * 8 + 7] * inv);
	}

	for each (GLuint var in plane_indices)
	{
		indices.push_back(var);
	}
	configureBuffers();
	setPosition(pos);
}

Cube::Cube(float xScale, float yScale, float zScale, bool inverseNormals)
{
	vertices.reserve(192);
	float inv = (inverseNormals) ? -1.0f : 1.0f;
	for (size_t i = 0; i < 24; ++i)
	{
		//x,y,z coordinates
		vertices.push_back(unit_cube[i * 8] * xScale);
		vertices.push_back(unit_cube[i * 8 + 1] * yScale);
		vertices.push_back(unit_cube[i * 8 + 2] * zScale);
		//texture coordinates
		vertices.push_back(unit_cube[i * 8 + 3]);
		vertices.push_back(unit_cube[i * 8 + 4]);
		//normals
		vertices.push_back(unit_cube[i * 8 + 5] * inv);
		vertices.push_back(unit_cube[i * 8 + 6] * inv);
		vertices.push_back(unit_cube[i * 8 + 7] * inv);
	}
	indices.reserve(36);

	for each (GLuint var in unit_cube_indices)
	{
		indices.push_back(var);
	}

	configureBuffers();
}

Cube::Cube(float xScale, float yScale, float zScale, std::string filePath, bool inverseNormals) : Cube(xScale,yScale,zScale,inverseNormals)
{
	texture = Texture(filePath);
}

Cube::Cube(float xScale, float yScale, float zScale, std::string filePath, glm::vec3 pos, bool inverseNormals): Cube(xScale,yScale,zScale,filePath,inverseNormals)
{
	setPosition(pos);
}

Cube::Cube(float xScale, float yScale, float zScale, GLuint texID, glm::vec3 pos, bool inverseNormals) : Cube(xScale, yScale, zScale,inverseNormals)
{
	texture = Texture(texID);
	setPosition(pos);
}

Cylinder::Cylinder(float height, float radius)
{
	Cylinder::generateSides(vertices, indices, radius, height, 0.0f, false);
	Cylinder::generateBase(vertices, indices, radius, height / 2, true);
	Cylinder::generateBase(vertices, indices, radius, -(height) / 2, false);
	configureBuffers();
}

Cylinder::Cylinder(float height, float radius, std::string filePath, glm::vec3 pos) : Cylinder(height,radius)
{
	texture = Texture(filePath);
	setPosition(pos);
}

Cylinder::Cylinder(float height, float radius, GLuint texID, glm::vec3 pos)
{
	texture = texID;
	setPosition(pos);
}

void Cylinder::generateSides(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, float height, float yTranslation, bool inverseNormal)
{
	float angle = 0;
	float currentTex = 0;
	float theta = (2 * PI) / CYLINDER_SIDES;
	float texStep = 1.0 / CYLINDER_SIDES;
	float sinus;
	float cosinus;
	float inv = (inverseNormal) ? -1.0f : 1.0f;
	unsigned int firstVertex, lastVertex;
	firstVertex = (vertices.size()) / 8;
	for (size_t i = 0; i <= CYLINDER_SIDES; ++i)
	{
		sinus = sin(angle);
		cosinus = cos(angle);
		//top vertices
		vertices.push_back(radius * cosinus);//x
		vertices.push_back((height / 2) + yTranslation);//y
		vertices.push_back(radius * sinus);//z
		vertices.push_back(currentTex);//x tex
		vertices.push_back(1.0);//y tex;
		vertices.push_back(cosinus*inv);//normal x
		vertices.push_back(0);//normal y
		vertices.push_back(sinus*inv);// normal z
		//bottom
		vertices.push_back(radius * cosinus);//x
		vertices.push_back((-height / 2) + yTranslation);//y
		vertices.push_back(radius * sinus);//z
		vertices.push_back(currentTex);//x tex
		vertices.push_back(0);//y tex;
		vertices.push_back(cosinus*inv);//normal x
		vertices.push_back(0);//normal y
		vertices.push_back(sinus*inv);// normal z

		angle += theta;
		currentTex += texStep;
	}
	lastVertex = firstVertex + ((CYLINDER_SIDES + 1) * 2);
	for (size_t i = firstVertex; i < lastVertex; i+=2)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 3);
		indices.push_back(i + 2);
	}
}

void Cylinder::generateBase(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, float height, bool normalUp)
{
	float angle = 0;
	float theta = (2 * PI) / CYLINDER_SIDES;
	float sinus;
	float cosinus;
	float up = (normalUp) ? 1.0f : -1.0f;
	unsigned int firstVertex, lastVertex, center;
	firstVertex = (vertices.size()) / 8;
	vertices.push_back(0);
	vertices.push_back(height);
	vertices.push_back(0);
	vertices.push_back(0.5);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	vertices.push_back(up);
	vertices.push_back(0.0);
	center = firstVertex;
	for (size_t i = 0; i <= CYLINDER_SIDES; ++i)
	{
		sinus = sin(angle);
		cosinus = cos(angle);
		vertices.push_back(radius * cosinus);//x
		vertices.push_back(height);//y
		vertices.push_back(radius * sinus);//z
		vertices.push_back(0.5 + 0.5 * cosinus);//tex x
		vertices.push_back(0.5 + 0.5 * sinus);//tex y
		vertices.push_back(0);//normal x
		vertices.push_back(up);//normal y
		vertices.push_back(0);// normal z
		angle += theta;
	}
	lastVertex = firstVertex + CYLINDER_SIDES + 1;
	for (size_t i = firstVertex; i <lastVertex; i++)
	{
		indices.push_back(center);//center
		indices.push_back(i);
		indices.push_back(i + 1);
	}
}

PressedCylinder::PressedCylinder(float height, float innerRadius, float outerRadius, float ratio)
{
	float angle = 0;
	float theta = (2 * PI) / CYLINDER_SIDES;
	float sinus;
	float cosinus;
	float outerTex = 0.5f;
	float innerTex = 0.5f * (innerRadius / outerRadius);
	float depth = ratio * height;
	Cylinder::generateBase(vertices, indices, outerRadius, -(height / 2), false);
	Cylinder::generateBase(vertices, indices, innerRadius, ((height / 2) - depth), true);
	Cylinder::generateSides(vertices, indices, outerRadius, height, 0.0f, false);
	Cylinder::generateSides(vertices, indices, innerRadius, depth, ((depth / 2) + ((height / 2) - depth)), true);
	unsigned int begin = (vertices.size()) / 8;
	for (size_t i = 0; i <= CYLINDER_SIDES; ++i)
	{
		sinus = sin(angle);
		cosinus = cos(angle);
		vertices.push_back(innerRadius * cosinus);//x
		vertices.push_back(height / 2);//y
		vertices.push_back(innerRadius * sinus);//z
		vertices.push_back(0.5 + innerTex * cosinus);
		vertices.push_back(0.5 + innerTex * sinus);
		vertices.push_back(0);//normal x
		vertices.push_back(1);//normal y
		vertices.push_back(0);// normal z

		vertices.push_back(outerRadius * cosinus);//x
		vertices.push_back(height / 2);//y
		vertices.push_back(outerRadius * sinus);//z
		vertices.push_back(0.5 + outerTex * cosinus);
		vertices.push_back(0.5 + outerTex * sinus);
		vertices.push_back(0);//normal x
		vertices.push_back(1);//normal y
		vertices.push_back(0);// normal z
		angle += theta;
	}
	unsigned int end = begin + (CYLINDER_SIDES * 2);
	for (size_t i = begin; i < end; i+=2)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 3);
		indices.push_back(i + 2);
	}
	configureBuffers();
}

PressedCylinder::PressedCylinder(float height, float innerRadius, float outerRadius, std::string filePath, glm::vec3 pos, float ratio) : PressedCylinder(height,innerRadius,outerRadius,ratio)
{
	texture = Texture(filePath);
	setPosition(pos);
}

PressedCylinder::PressedCylinder(float height, float innerRadius, float outerRadius, GLuint texID, glm::vec3 pos, float ratio) : PressedCylinder(height, innerRadius, outerRadius,ratio)
{
	texture = texID;
	setPosition(pos);
}

Lamp::Lamp(float xScale, float yScale, float zScale, glm::vec3 pos)
{
	vertices.reserve(72);
	for (size_t i = 0; i < 24; ++i)
	{
		//x,y,z coordinates
		vertices.push_back(unit_cube[i * 8] * xScale);
		vertices.push_back(unit_cube[i * 8 + 1] * yScale);
		vertices.push_back(unit_cube[i * 8 + 2] * zScale);
	}
	indices.reserve(36);

	for each (GLuint var in unit_cube_indices)
	{
		indices.push_back(var);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);//vertex position

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setPosition(pos);
}

Lamp::Lamp(float radius, glm::vec3 pos, int latitude , int longitude)
{
	float alpha = (PI / 2) / (latitude - 1);//latitude include pole
	int uLatitude = latitude - 2;
	float angle = PI/2;
	float theta = (2 * PI) / longitude;
	float sinus, cosinus, actR, height;
	float tmpangle = 0;
	int begin,begin2;
	//north pole
	vertices.push_back(0);
	vertices.push_back(radius);
	vertices.push_back(0);
	//south pole
	vertices.push_back(0);
	vertices.push_back(-radius);
	vertices.push_back(0);
	angle = alpha * uLatitude;
	actR = radius * cos(angle);
	height = radius * sin(angle);
	for (int i = 0; i <= longitude; ++i)
	{
		sinus = sin(tmpangle);
		cosinus = cos(tmpangle);
		vertices.push_back(actR * cosinus);//x
		vertices.push_back(height);//y
		vertices.push_back(actR * sinus);//z
		
		vertices.push_back(actR * cosinus);//x
		
		vertices.push_back(-height);//y
		vertices.push_back(actR * sinus);//z
		tmpangle += theta;
	}
	for (int i = 0; i <= longitude; ++i)
	{
		indices.push_back(0);
		indices.push_back(2 + i * 2);
		indices.push_back(2 + i * 2 + 2);
		indices.push_back(1);
		indices.push_back(3 + i * 2);
		indices.push_back(3 + i * 2 + 2);
	}
	begin = 2;
	if (latitude > 2)
	{
		--uLatitude;
		angle = alpha * uLatitude;
		while (angle >= 0)
		{
			actR = radius * cos(angle);
			height = radius * sin(angle);
			tmpangle = 0;
			for (int i = 0; i <= longitude; ++i)
			{
				sinus = sin(tmpangle);
				cosinus = cos(tmpangle);
				vertices.push_back(actR * cosinus);//x
				vertices.push_back(height);//y
				vertices.push_back(actR * sinus);//z

				vertices.push_back(actR * cosinus);//x
				vertices.push_back(-height);//y
				vertices.push_back(actR * sinus);//z
				tmpangle += theta;
			}
			begin2 = begin + 2*(longitude + 1);
			for (int i = begin; i < begin + 2*(longitude+1); i+=2, begin2+=2)
			{
				indices.push_back(i);
				indices.push_back(begin2);
				indices.push_back(i + 2);

				indices.push_back(i + 2);
				indices.push_back(begin2);
				indices.push_back(begin2 + 2);

				indices.push_back(begin2 + 1);
				indices.push_back(i + 1);
				indices.push_back(begin2 + 3);

				indices.push_back(begin2 + 3);
				indices.push_back(i + 1);
				indices.push_back(i + 3);
			}
			--uLatitude;
			angle = alpha * uLatitude;
			begin += 2*(longitude + 1);
		}
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);//vertex position

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setPosition(pos);
}
