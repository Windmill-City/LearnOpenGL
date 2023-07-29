#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Resource.hpp"

class Shader
{
  protected:
    unsigned int ID;

  public:
    Shader(ResourceStream vShader, ResourceStream fShader);
    ~Shader();

    Shader(Shader&& _Right);
    Shader(const Shader&) = delete;

    /**
     * @brief Set it to the current shader
     *
     */
    void use() const;
    /**
     * @brief Set uniform bool value
     *
     * @param name uniform name
     * @param value bool value
     */
    void setBool(const std::string& name, bool value) const;
    /**
     * @brief Set uniform int value
     *
     * @param name uniform name
     * @param value int value
     */
    void setInt(const std::string& name, int value) const;
    /**
     * @brief Set uniform float value
     *
     * @param name uniform name
     * @param value float value
     */
    void setFloat(const std::string& name, float value) const;

  protected:
    void checkCompileErrors(unsigned int shader, std::string type);
};