#ifndef SHADER_H
#define SHADER_H

#include <OpenGL.h>
#include "Globals.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
class Shaders
{
public:
	//Constructors/Destructors
	Shaders(const int versionMajorP, const int versionMinorP,
		char* vertexFile, char* fragmentFile, char* geometryFile = "");
	~Shaders();

	GLuint id;

	//Set uniform functions
	void use();
	void unuse();
    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;

private:
	//Member variables

	int versionMajor;
	int versionMinor;

	//Private functions
	std::string loadShaderSource(char* fileName);

	GLuint loadShader(GLenum type, char* fileName);

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
};
#endif
