#include "Shader.hpp"

Shader::Shader(ResourceStream vShader, ResourceStream fShader)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string   vertexCode;
    std::string       fragmentCode;
    std::stringstream vShaderStream, fShaderStream;

    // read file's buffer contents into streams
    vShaderStream << vShader->rdbuf();
    fShaderStream << fShader->rdbuf();
    // convert stream into string
    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char*  vShaderCode = vertexCode.c_str();
    const char*  fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

Shader::Shader(Shader&& _Right)
    : ID(_Right.ID)
{
    _Right.ID = 0;
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string_view name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.data()), (int)value);
}

void Shader::setInt(const std::string_view name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}

void Shader::setFloat(const std::string_view name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.data()), value);
}
