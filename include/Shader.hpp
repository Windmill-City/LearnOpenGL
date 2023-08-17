#pragma once

#include "Resource.hpp"

struct Shader
{
  protected:
    unsigned int ProgramID;

  public:
    Shader() = default;
    ~Shader();

    Shader(Shader&& _right);
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
    void setBool(const std::string_view name, bool value) const;
    /**
     * @brief Set uniform int value
     *
     * @param name uniform name
     * @param value int value
     */
    void setInt(const std::string_view name, int value) const;
    /**
     * @brief Set uniform float value
     *
     * @param name uniform name
     * @param value float value
     */
    void setFloat(const std::string_view name, float value) const;
};