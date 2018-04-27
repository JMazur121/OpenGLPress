#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
class ShaderProgram
{
public:
	ShaderProgram() {}
	// Constructor reads and builds the shader
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);

	// Use the program
	void use() const
	{
		glUseProgram(get_programID());
	}

	// returns program ID
	GLuint get_programID() const
	{
		return program_id;
	}

private:
	GLuint program_id; 	// The program ID
};
