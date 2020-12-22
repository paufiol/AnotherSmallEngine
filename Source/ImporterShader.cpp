#include "Application.h"
#include "ImporterShader.h"
#include "ResourceShader.h"
#include "ModuleFileSystem.h"

#include "Dependencies/Glew/include/GL/glew.h"
#include "Dependencies/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Dependencies/Glew/libx86/Release/Win32/glew32.lib") 


void Importer::ShaderImporter::Import(const char* fullPath, ResourceShader* shader)
{
	std::string path;
	std::string name;
	std::string ext;
	App->fileSystem->SplitFilePath(fullPath, &path, &name, &ext);

	int vertexShader, fragmentShader;

	char* buffer;
	int size = App->fileSystem->Load(fullPath, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Shader: %s not found or can't be loaded.", fullPath);
		return;
	}
	
	// This is a temporal solution to the 2 paths issue

	//Case the path points to th vertex shader import it and get the fragment one
	if (ext == "vert")
	{
		vertexShader = ImportVertex(buffer, shader);

		RELEASE_ARRAY(buffer);

		std::string fragPath = path + name + ".frag";

		size = App->fileSystem->Load(fragPath.c_str(), &buffer);

		if (size <= 0)
		{
			delete[] buffer;
			LOG("Shader: %s not found or can't be loaded.", fragPath);
			return;
		}
		else
		{
			fragmentShader = ImportFragment(buffer, shader);
		}
	}
	//Case the path points to th fragment shader import it and get the vertex one
	else if (ext == "frag")
	{
		fragmentShader = ImportFragment(buffer, shader);


		RELEASE_ARRAY(buffer);

		std::string vertexPath = path + name + ".vert";

		size = App->fileSystem->Load(vertexPath.c_str(), &buffer);

		if (size <= 0)
		{
			delete[] buffer;
			LOG("Shader: %s not found or can't be loaded.", vertexPath);
			return;
		}
		else
		{
			vertexShader = ImportVertex(buffer, shader);
		}
	}
	// Create the Shader Program and link it
	if (vertexShader != -1 && fragmentShader != -1)
	{
		GLint outcome;
		vertexShader = (GLuint)vertexShader;
		fragmentShader = (GLuint)fragmentShader;

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &outcome);
		if (outcome == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, info);
			LOG("Shader compiling error: %s", info);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(shaderProgram);
	}
	

}

int Importer::ShaderImporter::ImportVertex(const char* buffer, ResourceShader* shader)
{
	std::string shaderFile(buffer);
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)shaderFile.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	glCompileShader(vertexShader);
	
	GLint outcome;
	GLchar info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}

	delete[] buffer;

	return (outcome != 0) ? vertexShader : -1;
}

int Importer::ShaderImporter::ImportFragment(const char* buffer, ResourceShader* shader)
{
	std::string shaderFile(buffer);
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)shaderFile.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	glCompileShader(fragmentShader);

	GLint outcome;
	GLchar info[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}

	delete[] buffer;

	return (outcome != 0) ? fragmentShader : -1;
}
