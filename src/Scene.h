#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "AABB.h"

// Один объект сцены: куб, шар или модель из OBJ(позже)
class SceneObject {
public:
    int id = 0;
    std::string name;
    Model model{};
    Vector3 position{0, 0, 0};   //центр объекта
    Vector3 scale{1, 1, 1};
    Color color = GRAY;
    AABB worldBox;

    void UpdateAABB();           // пересчитать AABB после перемещения
};

// Набор объектов
class Scene {
public:
    std::vector<SceneObject> objects;

    int  AddBox(const std::string& name, Vector3 center, Vector3 size, Color c);
    int  AddSphere(const std::string& name, Vector3 center, float radius, Color c);
    void Clear();
    void LoadTestScene();                 
    void GenerateRandom(int count, unsigned seed);

    std::vector<AABB> CollectBoxes() const;     
    AABB Bounds() const;                         
};
