#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

struct TriRenderer
{
    const Shader shader;

    TriRenderer(Shader shader);
    void render();
};