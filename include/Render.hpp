#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

struct Render
{
    const Shader shader;

    Render(Shader shader);
    void render();
};