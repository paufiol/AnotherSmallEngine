#include "Shaders.h"


Shaders::Shaders(const int versionMajorP, const int versionMinorP,
    const char* vertexFile, const char* fragmentFile, char* geometryFile)
{
    versionMajor = versionMajorP;
    versionMinor = versionMinorP;
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragmentShader = 0;

    vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

    if (geometryFile != "")
        geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

    this->linkProgram(vertexShader, geometryShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}
Shaders::~Shaders()
{
    glDeleteProgram(this->id);
}

std::string Shaders::loadShaderSource(const char* fileName)
{
    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    //Vertex
    in_file.open(fileName);

    if (in_file.is_open())
    {
        while (std::getline(in_file, temp))
            src += temp + "\n";
    }
    else
    {
        std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
    }

    in_file.close();

    std::string versionNr =
        std::to_string(this->versionMajor) +
        std::to_string(this->versionMinor) +
        "0";

    src.replace(src.find("#version"), 12, ("#version " + versionNr));

    return src;
}
GLuint Shaders::loadShader(GLenum type, const char* fileName)
{
    char infoLog[512];
    GLint success;

    GLuint shader = glCreateShader(type);
    std::string str_src = this->loadShaderSource(fileName);
    const GLchar* src = str_src.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
        std::cout << infoLog << "\n";
    }

    return shader;
}
void Shaders::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
{
    char infoLog[512];
    GLint success;

    this->id = glCreateProgram();

    glAttachShader(this->id, vertexShader);

    if (geometryShader)
        glAttachShader(this->id, geometryShader);

    glAttachShader(this->id, fragmentShader);

    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
        std::cout << infoLog << "\n";
    }

    glUseProgram(0);
}

void Shaders::use()
{
    glUseProgram(this->id);
}

void Shaders::unuse()
{
    glUseProgram(0);
}
void Shaders::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shaders::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shaders::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
    
}