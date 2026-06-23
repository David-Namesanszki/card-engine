#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct RenderObject {
    std::string   texture;
    std::string   text;
    Color         color    = WHITE;
    int           layer    = 0;
    bool          visible  = true;
    
    RenderObject*              parent   = nullptr;
    std::vector<RenderObject*> children;

    void addObject(const RenderObject* object);
};
