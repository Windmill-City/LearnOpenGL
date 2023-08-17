#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

struct Renderer
{
    Shader DefaultShader;

    Renderer();
    void newFrame();
};