#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL.h>
#include <string>

class Texture
{
public:
	Texture(){}
	Texture(GLuint id);
	Texture(std::string filePath);
	GLuint getID();
private:
	GLuint textureID;
};