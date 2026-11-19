#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "Scene.h"
#include "Theme.h"

// Отрисовка: 3D-сцена и текст. Все получает параметрами
class Renderer {
public:
    bool showAABB = true;

    void Init();       
    void Shutdown();

    // colliding[i] == true — объект i сейчас участвует в коллизии
    void DrawScene(const Scene& scene, const std::vector<bool>& colliding,
                   int selected, const Camera3D& cam) const;

    void Text(const std::string& s, float x, float y, float size, Color c) const;
    float TextWidth(const std::string& s, float size) const;

private:
    Font font{};
    bool ownFont = false;
    void DrawGroundGrid(const AABB& area) const;
};
