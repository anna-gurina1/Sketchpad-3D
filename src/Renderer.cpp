#include "Renderer.h"
#include <cmath>

void Renderer::Init()
{
    // шрифт для кирилицы
    const char* path = "C:/Windows/Fonts/arial.ttf";
    if (FileExists(path)) {
        std::vector<int> cps;
        for (int c = 32; c < 127; c++) cps.push_back(c);           // латиница, цифры
        for (int c = 0x400; c <= 0x45F; c++) cps.push_back(c);     // кириллица
        cps.push_back(0x2013); cps.push_back(0x2014); cps.push_back(0x2192); // – — →
        font = LoadFontEx(path, 40, cps.data(), (int)cps.size());
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
        ownFont = true;
    } else {
        font = GetFontDefault();
    }
}

void Renderer::Shutdown()
{
    if (ownFont) UnloadFont(font);
}

void Renderer::Text(const std::string& s, float x, float y, float size, Color c) const
{
    DrawTextEx(font, s.c_str(), { x, y }, size, 0.0f, c);
}

float Renderer::TextWidth(const std::string& s, float size) const
{
    return MeasureTextEx(font, s.c_str(), size, 0.0f).x;
}

void Renderer::DrawGroundGrid(const AABB& area) const
{
    // сетка в плоскости z = 0 (DrawGrid из raylib рисует в XZ, а тут сделаем Z - верх)
    int x0 = (int)floorf(area.min.x) - 2, x1 = (int)ceilf(area.max.x) + 2;
    int y0 = (int)floorf(area.min.y) - 2, y1 = (int)ceilf(area.max.y) + 2;
    int step = (x1 - x0 > 60) ? 5 : 1;
    Color line = { 215, 219, 225, 255 };
    for (int x = x0; x <= x1; x += step) DrawLine3D({ (float)x, (float)y0, 0 }, { (float)x, (float)y1, 0 }, line);
    for (int y = y0; y <= y1; y += step) DrawLine3D({ (float)x0, (float)y, 0 }, { (float)x1, (float)y, 0 }, line);
    DrawLine3D({ 0, 0, 0.01f }, { 2, 0, 0.01f }, RED);     // ось X
    DrawLine3D({ 0, 0, 0.01f }, { 0, 2, 0.01f }, GREEN);   // ось Y
    DrawLine3D({ 0, 0, 0 },     { 0, 0, 2 },     BLUE);    // ось Z
}

void Renderer::DrawScene(const Scene& scene, const std::vector<bool>& colliding,
                         int selected, const Camera3D& cam) const
{
    BeginMode3D(cam);
    DrawGroundGrid(scene.Bounds());

    const Color hit = { 220, 50, 47, 255 };
    for (size_t i = 0; i < scene.objects.size(); i++)
    {
        const SceneObject& o = scene.objects[i];
        bool isHit = i < colliding.size() && colliding[i];
        Color c = isHit ? hit : o.color;

        DrawModelEx(o.model, o.position, { 0, 0, 1 }, 0.0f, o.scale, c);
        DrawModelWiresEx(o.model, o.position, { 0, 0, 1 }, 0.0f, o.scale, Fade(BLACK, 0.25f));

        if (showAABB)
            DrawBoundingBox(o.worldBox.ToBoundingBox(), isHit ? MAROON : Fade(DARKGRAY, 0.6f));
        if ((int)i == selected)
            DrawBoundingBox(o.worldBox.ToBoundingBox(), YELLOW);
    }
    EndMode3D();

    // подписи объектов (для небольших сцен)
    if (scene.objects.size() <= 30)
        for (const auto& o : scene.objects) {
            Vector3 top = { o.position.x, o.position.y, o.worldBox.max.z + 0.3f };
            Vector2 p = GetWorldToScreen(top, cam);
            Text(o.name, p.x - 6, p.y - 12, 22, BLACK);
        }
}
