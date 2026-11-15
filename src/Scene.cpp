#include "Scene.h"
#include <random>
#include <cmath>

void SceneObject::UpdateAABB()
{
    // у простых фигур одна сетка; у моделей OBJ их может быть несколько
    worldBox = ComputeAABB(model.meshes[0].vertices, model.meshes[0].vertexCount, position, scale);
    for (int k = 1; k < model.meshCount; k++)
        worldBox = AABB::Merge(worldBox,
            ComputeAABB(model.meshes[k].vertices, model.meshes[k].vertexCount, position, scale));
}

static int AddModel(Scene& s, const std::string& name, Model model, Vector3 center, Color c)
{
    SceneObject o;
    o.id = (int)s.objects.size();
    o.name = name;
    o.model = model;
    o.position = center;
    o.color = c;
    o.UpdateAABB();
    s.objects.push_back(o);
    return o.id;
}

int Scene::AddBox(const std::string& name, Vector3 center, Vector3 size, Color c)
{
    // сетка с центром в нуле
    Model m = LoadModelFromMesh(GenMeshCube(size.x, size.y, size.z));
    return AddModel(*this, name, m, center, c);
}

int Scene::AddSphere(const std::string& name, Vector3 center, float radius, Color c)
{
    Model m = LoadModelFromMesh(GenMeshSphere(radius, 16, 16));
    return AddModel(*this, name, m, center, c);
}

void Scene::Clear()
{
    for (auto& o : objects) UnloadModel(o.model);
    objects.clear();
}

void Scene::LoadTestScene()
{
    Clear();
    struct Item { const char* n; float x0, x1, y0, y1; Color c; };
    const Item items[] = {
        { "A", 1, 3, 1, 3, { 47, 109, 181, 255 } },
        { "B", 2, 4, 2, 4, { 47, 109, 181, 255 } },
        { "C", 6, 7, 1, 2, { 140, 146, 155, 255 } },
        { "D", 6, 8, 4, 6, { 217, 130, 43, 255 } },
        { "E", 7, 9, 5, 7, { 217, 130, 43, 255 } },
        { "F", 1, 2, 6, 7, { 140, 146, 155, 255 } },
    };
    for (const auto& it : items)
        AddBox(it.n, { (it.x0 + it.x1) / 2, (it.y0 + it.y1) / 2, 0.5f },
                     { it.x1 - it.x0, it.y1 - it.y0, 1.0f }, it.c);
}

void Scene::GenerateRandom(int count, unsigned seed)
{
    Clear();
    std::mt19937 rng(seed);
    const float minSize = 0.5f, maxSize = 2.0f;   // диапазон размеров объектов
    const float spacing = 3.2f;                   // плотность: во сколько раз шаг между объектами больше их размера
    float avgSize = (minSize + maxSize) / 2;
    float side = spacing * avgSize * std::sqrt((float)count);   // сторона области ~ √count: плотность не зависит от числа объектов
    std::uniform_real_distribution<float> pos(0, side), size(0.5f, 2.0f), h(0, 3);
    std::uniform_int_distribution<int> kind(0, 3);
    for (int i = 0; i < count; i++)
    {
        Vector3 c = { pos(rng), pos(rng), h(rng) };
        Color col = { (unsigned char)(80 + rng() % 120), (unsigned char)(110 + rng() % 100), (unsigned char)(150 + rng() % 90), 255 };
        std::string name = "#" + std::to_string(i);
        if (kind(rng) == 0) AddSphere(name, c, size(rng) * 0.5f, col);
        else AddBox(name, c, { size(rng), size(rng), size(rng) }, col);
    }
}

std::vector<AABB> Scene::CollectBoxes() const
{
    std::vector<AABB> boxes;
    boxes.reserve(objects.size());
    for (const auto& o : objects) boxes.push_back(o.worldBox);
    return boxes;
}

AABB Scene::Bounds() const
{
    if (objects.empty()) return { {0, 0, 0}, {10, 10, 1} };
    AABB b = objects[0].worldBox;
    for (const auto& o : objects) b = AABB::Merge(b, o.worldBox);
    return b;
}
