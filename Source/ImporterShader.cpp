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

	int vertexShader = -1, fragmentShader = -1;

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

		shader->vertexID = vertexShader;

		////RELEASE_ARRAY(buffer);

		//std::string fragPath = path + name + ".frag";

		//size = App->fileSystem->Load(fragPath.c_str(), &buffer);

		//if (size <= 0)
		//{
		//	delete[] buffer;
		//	LOG("Shader: %s not found or can't be loaded.", fragPath);
		//	return;
		//}
		//else
		//{
		//	fragmentShader = ImportFragment(buffer, shader);
		//}
	}
	//Case the path points to th fragment shader import it and get the vertex one
	else if (ext == "frag")
	{
		fragmentShader = ImportFragment(buffer, shader);

		shader->fragmentID = fragmentShader;
		////RELEASE_ARRAY(buffer);

		//std::string vertexPath = path + name + ".vert";

		//size = App->fileSystem->Load(vertexPath.c_str(), &buffer);

		//if (size <= 0)
		//{
		//	delete[] buffer;
		//	LOG("Shader: %s not found or can't be loaded.", vertexPath);
		//	return;
		//}
		//else
		//{
		//	vertexShader = ImportVertex(buffer, shader);
		//}
	}
	//// Create the Shader Program and link it
	if (shader->vertexID != -1 && shader->fragmentID != -1)
	{
		GLint outcome;
		vertexShader = (GLuint)vertexShader;
		fragmentShader = (GLuint)fragmentShader;

		shader->shaderProgramID = glCreateProgram();
		glAttachShader(shader->shaderProgramID, vertexShader);
		glAttachShader(shader->shaderProgramID, fragmentShader);
		glLinkProgram(shader->shaderProgramID);

		glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &outcome);
		if (outcome == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shader->shaderProgramID, 512, NULL, info);
			LOG("Shader compiling error: %s", info);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(shader->shaderProgramID);
	}
	else
	{
		LOG("ERROR, Vertex shader: &d or Fragment shader: %d are not correctly compiled.", vertexShader, fragmentShader);
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
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

uint64 Importer::ShaderImporter::Save(const ResourceShader* shader, char** buffer)
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	if (formats < 1) return 0;
	
	
	char* binaryBuffer = nullptr;
	GLint binarySize = 0;
	int fullSize = 0;

	glGetProgramiv(shader->shaderProgramID, GL_PROGRAM_BINARY_LENGTH, &binarySize);
	if(binarySize > 0)
	{
		fullSize = sizeof(int) + binarySize;
		*buffer = new char[fullSize];
		binaryBuffer = new char[binarySize];

		GLsizei lenght;
		GLenum binaryFormat;

		glGetProgramBinary(shader->shaderProgramID, binarySize, &lenght, &binaryFormat, binaryBuffer);

		char* cursor = *buffer;
		memcpy(cursor, &binaryFormat, sizeof(int));
		cursor += sizeof(int);

		memcpy(cursor, binaryBuffer, binarySize);
		RELEASE_ARRAY(binaryBuffer);
	}
		
	return fullSize;
}

void Importer::ShaderImporter::Load(ResourceShader* shader, const char* buffer, uint size)
{
	GLenum binaryFormat;

	const char* cursor = buffer;
	memcpy(&binaryFormat, cursor, sizeof(unsigned int));
	cursor += sizeof(unsigned int);

	shader->shaderProgramID = glCreateProgram();
	glProgramBinary(shader->shaderProgramID, 36385, cursor, size - sizeof(unsigned int));

	GLint state;
	glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &state);

	if (state == GL_FALSE)
	{
		char str[512];
		glGetProgramInfoLog(shader->shaderProgramID, 512, nullptr, str);
		LOG("Shader Compilation Error: %s", str);
		glDeleteProgram(shader->shaderProgramID);
	}
}
