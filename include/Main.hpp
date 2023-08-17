#pragma once

#include "Render.hpp"

struct MainContext
{
    static MainContext Main;

    ResourceManager R;
    Renderer        Render;

    void setup();
};