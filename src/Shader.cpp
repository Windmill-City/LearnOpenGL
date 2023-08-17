#include "Shader.hpp"

#include <glad/glad.h>

Shader::~Shader()
{
    glDeleteProgram(ProgramID);
}

Shader::Shader(Shader&& _right)
    : ProgramID(_right.ProgramID)
{
    _right.ProgramID = 0;
}

void Shader::use() const
{
    glUseProgram(ProgramID);
}

void Shader::setBool(const std::string_view name, bool value) const
{
    glUniform1i(glGetUniformLocation(ProgramID, name.data()), (int)value);
}

void Shader::setInt(const std::string_view name, int value) const
{
    glUniform1i(glGetUniformLocation(ProgramID, name.data()), value);
}

void Shader::setFloat(const std::string_view name, float value) const
{
    glUniform1f(glGetUniformLocation(ProgramID, name.data()), value);
}
