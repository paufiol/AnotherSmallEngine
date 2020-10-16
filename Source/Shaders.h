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
    unsigned int ID;
    // constructor generates the shader on the fly
    Shaders(const char* vertexPath, const char* fragmentPath);
    ~Shaders();
    void use();
    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;

private:
    // Check for errors
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
