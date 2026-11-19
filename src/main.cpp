#include <chrono>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <vector>
#include "raylib.h"
#include "Scene.h"
#include "Renderer.h"
#include "Panel.h"
#include "OrbitCamera.h"
#include "Tests.h"
#include "Theme.h"
#include "WindowStyle.h"
#include "BruteForceDetector.h"

int main()
{
#ifdef _WIN32
    std::system("chcp 65001 > nul");   // русские буквы в консоли 
#endif
    RunAllTests();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Sketchpad 3D");
    ApplyTitleBarStyle(theme.titleBar, theme.titleText);   // тёмная полоса заголовка
    MaximizeWindow();
    SetTargetFPS(60);

    Renderer renderer;
    renderer.Init();
    Panel panel;
    OrbitCamera orbit;

    Scene scene;
    scene.LoadTestScene();
    AABB b = scene.Bounds();
    orbit.Frame(b.min, b.max);

    //алгоритмы
    std::vector<std::unique_ptr<CollisionDetector>> detectors;
    detectors.push_back(std::make_unique<BruteForceDetector>());

    int activeAlgorithm = -1;            // пока ни один не выбран — коллизии не ищем
    bool hasMeasurement = false;
    AlgorithmMetrics measured;           // замер - только по нажатию кнопки
    std::vector<CollisionPair> live;     // текущие коллизии - каждый кадр

    int selected = -1;                   // выбранный объект
    bool dragging = false;
    float dragZ = 0;                     // высота плоскости, по которой тащим
    Vector3 grabOffset{};

    while (!WindowShouldClose())
    {
        Rectangle panelArea = panel.Area();
        Vector2 mouse = GetMousePosition();
        bool mouseInView = mouse.x < panelArea.x;

        orbit.Update(mouseInView);
        Camera3D cam = orbit.Get();

        // выбор и перетаскивание объекта мышью
        if (mouseInView && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Ray ray = GetScreenToWorldRay(mouse, cam);
            selected = -1;
            float best = 1e30f;
            for (int i = 0; i < (int)scene.objects.size(); i++) {
                RayCollision rc = GetRayCollisionBox(ray, scene.objects[i].worldBox.ToBoundingBox());
                if (rc.hit && rc.distance < best) { best = rc.distance; selected = i; }
            }
            if (selected >= 0) {
                SceneObject& o = scene.objects[selected];
                dragging = true;
                dragZ = o.position.z;
                float t = (dragZ - ray.position.z) / ray.direction.z;
                Vector3 p = { ray.position.x + ray.direction.x * t, ray.position.y + ray.direction.y * t, dragZ };
                grabOffset = { o.position.x - p.x, o.position.y - p.y, 0 };
            }
        }
        if (dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && selected >= 0) {
            Ray ray = GetScreenToWorldRay(mouse, cam);
            if (fabsf(ray.direction.z) > 1e-5f) {
                float t = (dragZ - ray.position.z) / ray.direction.z;   // пересечение луча с плоскостью z = dragZ
                if (t > 0) {
                    SceneObject& o = scene.objects[selected];
                    o.position.x = ray.position.x + ray.direction.x * t + grabOffset.x;
                    o.position.y = ray.position.y + ray.direction.y * t + grabOffset.y;
                    o.UpdateAABB();
                }
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) dragging = false;

        if (selected >= 0) {
            float dz = (IsKeyDown(KEY_E) ? 1.0f : 0.0f) - (IsKeyDown(KEY_Q) ? 1.0f : 0.0f);
            if (dz != 0) {
                scene.objects[selected].position.z += dz * 3.0f * GetFrameTime();
                scene.objects[selected].UpdateAABB();
            }
        }

        //постоянная проверка коллизий
        if (activeAlgorithm >= 0) {
            AlgorithmMetrics tmp;
            live = detectors[activeAlgorithm]->Detect(scene.CollectBoxes(), tmp);
        } else {
            live.clear();
        }
        std::vector<bool> colliding(scene.objects.size(), false);
        for (const auto& p : live) { colliding[p.a] = true; colliding[p.b] = true; }

        BeginDrawing();
        ClearBackground(theme.backgroundBottom);
        renderer.DrawScene(scene, colliding, selected, cam);

        PanelState ps;
        for (auto& d : detectors) ps.algorithms.push_back(d->Name());
        ps.activeAlgorithm = activeAlgorithm;
        ps.hasMeasurement = hasMeasurement;
        ps.measured = measured;
        ps.objectCount = (int)scene.objects.size();
        ps.liveCollisions = live;
        ps.scene = &scene;
        ps.selected = selected;
        ps.showAABB = renderer.showAABB;
        PanelActions act = panel.Draw(renderer, ps);
        EndDrawing();

        //реакция на кнопки
        if (act.algorithmClicked >= 0) {
            activeAlgorithm = act.algorithmClicked;
            std::vector<AABB> boxes = scene.CollectBoxes();   // сбор коробок не входит в замер
            measured.Reset();
            auto t0 = std::chrono::steady_clock::now();
            live = detectors[activeAlgorithm]->Detect(boxes, measured);
            auto t1 = std::chrono::steady_clock::now();
            measured.timeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
            hasMeasurement = true;
        }
        if (act.loadTestScene || act.randomCount > 0) {
            if (act.loadTestScene) scene.LoadTestScene();
            else scene.GenerateRandom(act.randomCount, (unsigned)GetRandomValue(1, 1000000));
            selected = -1; dragging = false; hasMeasurement = false;
            AABB bb = scene.Bounds();
            orbit.Frame(bb.min, bb.max);
        }
        if (act.toggleAABB) renderer.showAABB = !renderer.showAABB;
    }

    scene.Clear();
    renderer.Shutdown();
    CloseWindow();
    return 0;
}
