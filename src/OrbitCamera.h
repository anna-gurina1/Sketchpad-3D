#pragma once
#include <cmath>
#include "raylib.h"

// Камера, которая вращается вокруг точки target
// Правая кнопка мыши - вращение, колесо - приближение, средняя кнопка - сдвиг
struct OrbitCamera {
    Vector3 target{5, 4, 0};
    float distance = 18.0f;
    float yaw = -2.2f;     // поворот вокруг вертикали, радианы
    float pitch = 0.75f;   // наклон над горизонтом, радианы

    Camera3D Get() const {
        Camera3D cam{};
        cam.target = target;
        cam.position = { target.x + distance * cosf(pitch) * cosf(yaw),
                         target.y + distance * cosf(pitch) * sinf(yaw),
                         target.z + distance * sinf(pitch) };
        cam.up = { 0, 0, 1 };
        cam.fovy = 45.0f;
        cam.projection = CAMERA_PERSPECTIVE;
        return cam;
    }

    void Update(bool mouseInViewport) {
        Vector2 d = GetMouseDelta();
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            yaw   -= d.x * 0.006f;
            pitch += d.y * 0.006f;
            if (pitch > 1.5f)  pitch = 1.5f;
            if (pitch < 0.05f) pitch = 0.05f;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            // сдвиг цели в плоскости земли вдоль направлений камеры
            float k = distance * 0.0015f;
            target.x += ( sinf(yaw) * d.x + cosf(yaw) * d.y) * k;
            target.y += (-cosf(yaw) * d.x + sinf(yaw) * d.y) * k;
        }
        if (mouseInViewport) {
            distance *= (1.0f - GetMouseWheelMove() * 0.1f);
            if (distance < 2.0f)   distance = 2.0f;
            if (distance > 500.0f) distance = 500.0f;
        }
    }

    //навести камеру на всю сцену
    void Frame(Vector3 min, Vector3 max) {
        target = { (min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2 };
        float dx = max.x - min.x, dy = max.y - min.y, dz = max.z - min.z;
        distance = sqrtf(dx * dx + dy * dy + dz * dz) * 1.3f + 4.0f;
    }
};
