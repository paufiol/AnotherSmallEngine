#include "Application.h"
#include "ImporterShader.h"
#include "ResourceShader.h"
#include "ModuleFileSystem.h"
#include"JsonConfig.h"

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

	char* buffer;
	int size = App->fileSystem->Load(fullPath, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Shader File: %s not found or can't be loaded.", fullPath);
		return;
	}
	
	std::string file(buffer);
	if (file.find(VERTEX_SHADER) != std::string::npos)
	{
		shader->vertexID = ImportVertex(file, shader);
	}
	if (file.find(FRAGMENT_SHADER) != std::string::npos)
	{
		shader->fragmentID = ImportFragment(file, shader);
	}

	delete[] buffer;

	// Create the Shader Program and link it
	if (shader->vertexID != 0 && shader->fragmentID != 0)
	{
		GLint outcome;
		shader->vertexID = (GLuint)shader->vertexID;
		shader->fragmentID = (GLuint)shader->fragmentID;

		shader->shaderProgramID = glCreateProgram();
		glAttachShader(shader->shaderProgramID, shader->vertexID);
		glAttachShader(shader->shaderProgramID, shader->fragmentID);
		glLinkProgram(shader->shaderProgramID);

		glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &outcome);
		if (outcome == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shader->shaderProgramID, 512, NULL, info);
			LOG("Shader compiling error: %s", info);
		}
		else if(shader->uniforms.size() == 0)
		{
			GetShaderUniforms(shader);
		}

		glDeleteShader(shader->vertexID);
		glDeleteShader(shader->fragmentID);
		glDeleteShader(shader->shaderProgramID);
	}
	else
	{
		LOG("ERROR, Vertex shader: %d or Fragment shader: %d are not correctly compiled.", shader->vertexID, shader->fragmentID);
	}
	

}

int Importer::ShaderImporter::ImportVertex(std::string shaderFile, ResourceShader* shader)
{
	std::string vertexFile = std::string ("#version 330 core\r\n") + 
		std::string("#define ") +
		VERTEX_SHADER + "\r\n";
	vertexFile += shaderFile;

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)vertexFile.c_str();
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);
	
	GLint outcome;
	GLchar info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}

	

	return (outcome != 0) ? vertexShader : -1;
}

int Importer::ShaderImporter::ImportFragment(std::string shaderFile, ResourceShader* shader)
{
	std::string fragmentFile = std::string("#version 330 core\r\n") +
		std::string("#define ") + 
		FRAGMENT_SHADER + "\r\n";
	fragmentFile += shaderFile;
	
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* source = (const GLchar*)fragmentFile.c_str();
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);

	GLint outcome;
	GLchar info[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &outcome);
	if (outcome == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		LOG("Vertex shader compilation error (%s)", info);
	}
 

	return (outcome != 0) ? fragmentShader : -1;
}

void Importer::ShaderImporter::GetShaderUniforms(ResourceShader* shader)
{
	GLint activeUniforms;

	glGetProgramiv(shader->shaderProgramID, GL_ACTIVE_UNIFORMS, &activeUniforms);

	for (uint i = 0; i < activeUniforms; i++)
	{
		Uniform uniform;
		GLint length;
		GLint size;
		GLchar name[32];
		glGetActiveUniform(shader->shaderProgramID, i, sizeof(name), &length, &size, &uniform.GLtype, name);
		uniform.name = name;
		if (uniform.name != "inColor" && uniform.name != "time" && uniform.name != "modelMatrix" && uniform.name != "viewMatrix" && uniform.name != "projectionMatrix" && uniform.name != "cameraPosition")
		{
			uint uinformLoc = glGetUniformLocation(shader->shaderProgramID, uniform.name.c_str());

			switch (uniform.GLtype)
			{
			case GL_INT:
				uniform.uniformType = UniformType::INT;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, &uniform.integer);
				break;
			case GL_FLOAT:
				uniform.uniformType = UniformType::FLOAT;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.floatNumber);
				break;
			case GL_BOOL:
				uniform.uniformType = UniformType::BOOL;
				//glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.boolean);
				break;
			case GL_INT_VEC2:
				uniform.uniformType = UniformType::INT_VEC2;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec2);
				//glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec2.y);
				break;
			case GL_INT_VEC3:
				uniform.uniformType = UniformType::INT_VEC3;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec3);
				/*glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec3.y);
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec3.z);*/
				break;
			case GL_INT_VEC4:
				uniform.uniformType = UniformType::INT_VEC4;
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec4);
				/*glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec4.y);
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec4.z);
				glGetUniformiv(shader->shaderProgramID, uinformLoc, (GLint*)&uniform.vec4.w);*/
				break;
			case GL_FLOAT_VEC2:
				uniform.uniformType = UniformType::FLOAT_VEC2;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, (GLfloat*)&uniform.vec2);
				//glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.vec2.y);
				break;
			case GL_FLOAT_VEC3:
				uniform.uniformType = UniformType::FLOAT_VEC3;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, (GLfloat*)&uniform.vec3);
				/*glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.vec3.y);
				glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.vec3.z);*/
				break;
			case GL_FLOAT_VEC4:
				uniform.uniformType = UniformType::FLOAT_VEC4;
				glGetUniformfv(shader->shaderProgramID, uinformLoc, (GLfloat*)&uniform.vec4);
				/*glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.vec4.y);
				glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.vec4.z);
				glGetUniformfv(shader->shaderProgramID, uinformLoc, &uniform.vec4.w);*/
				break;
			case GL_FLOAT_MAT4:
				uniform.uniformType = UniformType::MATRIX4;
				glGetnUniformfv(shader->shaderProgramID, uinformLoc, sizeof(uniform.matrix4), &uniform.matrix4.v[0][0]);
				break;

			default: uniform.uniformType = UniformType::NONE; break;

			}

			if(uniform.uniformType != UniformType::NONE) shader->uniforms.push_back(uniform);
			
		}

		
	}
}

void Importer::ShaderImporter::SetShaderUniforms(ResourceShader* shader)
{
	if (shader->uniforms.empty())
	{
		return;
	}

	for (uint i = 0; i < shader->uniforms.size(); i++)
	{
		switch (shader->uniforms[i].uniformType)
		{
		//case UniformType::BOOL: shader->SetUniform1i(shader->uniforms[i].name.c_str(), shader->uniforms[i].boolean); break;
		case  UniformType::INT: shader->SetUniform1i(shader->uniforms[i].name.c_str(),shader->uniforms[i].integer); break;
		case  UniformType::FLOAT: shader->SetUniform1f(shader->uniforms[i].name.c_str(), shader->uniforms[i].floatNumber); break;
		case  UniformType::INT_VEC2: shader->SetUniformVec2i(shader->uniforms[i].name.c_str(), (GLint*)shader->uniforms[i].vec2.ptr()); break;
		case  UniformType::INT_VEC3: shader->SetUniformVec3i(shader->uniforms[i].name.c_str(), (GLint*)shader->uniforms[i].vec3.ptr()); break;
		case  UniformType::INT_VEC4: shader->SetUniformVec4i(shader->uniforms[i].name.c_str(), (GLint*)shader->uniforms[i].vec4.ptr()); break;
		case  UniformType::FLOAT_VEC2: shader->SetUniformVec2f(shader->uniforms[i].name.c_str(), (GLfloat*)shader->uniforms[i].vec2.ptr()); break;
		case  UniformType::FLOAT_VEC3: shader->SetUniformVec3f(shader->uniforms[i].name.c_str(), (GLfloat*)shader->uniforms[i].vec3.ptr()); break;
		case  UniformType::FLOAT_VEC4: shader->SetUniformVec4f(shader->uniforms[i].name.c_str(), (GLfloat*)shader->uniforms[i].vec4.ptr()); break;
		case UniformType::MATRIX4: shader->SetUniformMatrix4(shader->uniforms[i].name.c_str(), (GLfloat*)shader->uniforms[i].matrix4.Transposed().ptr()); break;
		default: break;
		}
	}
}

uint64 Importer::ShaderImporter::Save(const ResourceShader* shader, char** buffer)
{
	
	JsonConfig jsonFile;
	ArrayConfig jsonArrray = jsonFile.SetArray("Uniforms");

	for (uint i = 0; i < shader->uniforms.size(); i++)
	{
		JsonConfig& node = jsonArrray.AddNode();
		node.SetString("Name", shader->uniforms[i].name);
		node.SetInteger("Type", (int)shader->uniforms[i].uniformType);
		switch (shader->uniforms[i].uniformType)
		{
		//case  UniformType::BOOL: node.SetInteger("Value", shader->uniforms[i].boolean); break;
		case  UniformType::INT: node.SetInteger("Value", shader->uniforms[i].integer); break;
		case  UniformType::FLOAT: node.SetNumber("Value", shader->uniforms[i].floatNumber); break;
		case  UniformType::INT_VEC2: node.SetFloat2("Value", shader->uniforms[i].vec2); break;
		case  UniformType::INT_VEC3: node.SetFloat3("Value", shader->uniforms[i].vec3); break;
		case  UniformType::INT_VEC4: node.SetFloat4("Value", shader->uniforms[i].vec4); break;
		case  UniformType::FLOAT_VEC2: node.SetFloat2("Value", shader->uniforms[i].vec2); break;
		case  UniformType::FLOAT_VEC3: node.SetFloat3("Value", shader->uniforms[i].vec3); break;
		case  UniformType::FLOAT_VEC4: node.SetFloat4("Value", shader->uniforms[i].vec4); break;
		}
	}

	uint size = jsonFile.SerializeConfig(buffer);


	return size;

}

void Importer::ShaderImporter::Load(ResourceShader* shader, const char* buffer, uint size)
{
	
	JsonConfig jsonFile(buffer); //needs buffer??
	ArrayConfig jsonArrray = jsonFile.GetArray("Uniforms");
	Uniform uniform;
	for (uint i = 0; i < jsonArrray.GetSize(); i++)
	{
		JsonConfig node = jsonArrray.GetNode(i);
		uniform.name = node.GetString("Name");
		uniform.uniformType = (UniformType)node.GetInteger("Type");
		switch (uniform.uniformType)
		{
		//case  UniformType::BOOL: uniform.boolean = node.GetInteger("Value"); break;
		case  UniformType::INT: uniform.integer = node.GetInteger("Value"); break;
		case  UniformType::FLOAT: uniform.floatNumber = node.GetNumber("Value"); break;
		case  UniformType::INT_VEC2: uniform.vec2 = node.GetFloat2("Value"); break;
		case  UniformType::INT_VEC3: uniform.vec3 = node.GetFloat3("Value"); break;
		case  UniformType::INT_VEC4: uniform.vec4 = node.GetFloat4("Value"); break;
		case  UniformType::FLOAT_VEC2: uniform.vec2 = node.GetFloat2("Value"); break;
		case  UniformType::FLOAT_VEC3: uniform.vec3 = node.GetFloat3("Value"); break;
		case  UniformType::FLOAT_VEC4: uniform.vec4 = node.GetFloat4("Value"); break;
		}
		
		shader->uniforms.push_back(uniform);
	}

	Recompile(shader);

}

void Importer::ShaderImporter::Recompile(ResourceShader* shader)
{
	glDetachShader(shader->shaderProgramID, shader->vertexID);
	glDetachShader(shader->shaderProgramID, shader->fragmentID);
	glDeleteProgram(shader->shaderProgramID);

	Importer::ShaderImporter::Import(shader->assetsFile.c_str(), shader);
}
