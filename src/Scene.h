#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "AABB.h"

class SceneObject {
public:
    int id = 0;
    std::string name;
    Model model{};
    Vector3 position{0, 0, 0};
    Vector3 scale{1, 1, 1};
    Color color = GRAY;
    AABB worldBox;

    void UpdateAABB();
};

class Scene {
public:
    std::vector<SceneObject> objects;

    int  AddBox(Vector3 pos, Vector3 size, Color c);
    void Clear();
    void LoadTestScene();
};
