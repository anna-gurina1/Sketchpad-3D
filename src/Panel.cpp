#include "Panel.h"
#include <cstdio>

Rectangle Panel::Area() const
{
    return { GetScreenWidth() - width, 0, width, (float)GetScreenHeight() };
}

bool Panel::Button(const Renderer& r, Rectangle rc, const std::string& label, bool active) const
{
    bool hover = CheckCollisionPointRec(GetMousePosition(), rc);
    Color bg = active ? Color{ 47, 109, 181, 255 } : (hover ? Color{ 225, 230, 238, 255 } : Color{ 240, 242, 245, 255 });
    DrawRectangleRounded(rc, 0.25f, 6, bg);
    DrawRectangleRoundedLinesEx(rc, 0.25f, 6, 1.0f, active ? Color{ 47, 109, 181, 255 } : Color{ 200, 205, 212, 255 });
    float tw = r.TextWidth(label, 20);
    r.Text(label, rc.x + (rc.width - tw) / 2, rc.y + (rc.height - 20) / 2, 20, active ? WHITE : Color{ 30, 35, 40, 255 });
    return hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

static std::string Fmt(const char* f, double v) { char b[64]; snprintf(b, sizeof b, f, v); return b; }
static std::string FmtLL(long long v) { return std::to_string(v); }

PanelActions Panel::Draw(const Renderer& r, const PanelState& s) const
{
    PanelActions a;
    Rectangle area = Area();
    DrawRectangleRec(area, { 250, 251, 252, 255 });
    DrawLine((int)area.x, 0, (int)area.x, (int)area.height, { 205, 210, 218, 255 });

    const Color ink = { 30, 35, 40, 255 }, muted = { 107, 114, 128, 255 };
    float x = area.x + 18, y = 16, w = width - 36;

    r.Text("Sketchpad 3D", x, y, 28, ink); y += 40;

    //алгоритмы
    r.Text("Алгоритм поиска коллизий", x, y, 19, muted); y += 28;
    for (int i = 0; i < (int)s.algorithms.size(); i++) {
        if (Button(r, { x, y, w, 38 }, s.algorithms[i], i == s.activeAlgorithm)) a.algorithmClicked = i;
        y += 46;
    }

    //результаты замера 
    y += 6;
    r.Text("Последний замер", x, y, 19, muted); y += 28;
    if (!s.hasMeasurement) {
        r.Text("Нажмите на алгоритм,", x, y, 19, ink); y += 24;
        r.Text("чтобы выполнить проверку", x, y, 19, ink); y += 34;
    } else {
        r.Text("Время:  " + Fmt("%.4f", s.measured.timeMs) + " мс", x, y, 20, ink); y += 26;
        r.Text("Тестов AABB:  " + FmtLL(s.measured.aabbTests), x, y, 20, ink); y += 26;
        r.Text("Пар-кандидатов:  " + FmtLL(s.measured.candidatePairs), x, y, 20, ink); y += 34;
    }

    // ---- текущее состояние (обновляется каждый кадр)
    r.Text("Сейчас", x, y, 19, muted); y += 28;
    r.Text("Объектов:  " + std::to_string(s.objectCount), x, y, 20, ink); y += 26;
    Color hitColor = s.liveCollisions.empty() ? ink : Color{ 200, 40, 40, 255 };
    r.Text("Коллизий:  " + std::to_string(s.liveCollisions.size()), x, y, 20, hitColor); y += 28;
    if (s.scene) {
        int shown = 0;
        for (const auto& p : s.liveCollisions) {
            if (shown == 8) { r.Text("…", x + 10, y, 20, muted); y += 24; break; }
            r.Text(s.scene->objects[p.a].name + " – " + s.scene->objects[p.b].name, x + 10, y, 20, hitColor);
            y += 24; shown++;
        }
    }
    y += 10;

    // ---- сцена
    r.Text("Сцена", x, y, 19, muted); y += 28;
    if (Button(r, { x, y, w, 36 }, "Тестовая сцена A–F", false)) a.loadTestScene = true;
    y += 44;
    float hw = (w - 8) / 2;
    if (Button(r, { x, y, hw, 36 }, "100 объектов", false)) a.randomCount = 100;
    if (Button(r, { x + hw + 8, y, hw, 36 }, "1000 объектов", false)) a.randomCount = 1000;
    y += 44;
    if (Button(r, { x, y, w, 36 }, s.showAABB ? "Скрыть AABB" : "Показать AABB", false)) a.toggleAABB = true;
    y += 50;

    // ---- выбранный объект
    if (s.scene && s.selected >= 0) {
        const SceneObject& o = s.scene->objects[s.selected];
        r.Text("Выбран: " + o.name, x, y, 20, ink); y += 26;
        r.Text("(" + Fmt("%.1f", o.position.x) + "; " + Fmt("%.1f", o.position.y) + "; " + Fmt("%.1f", o.position.z) + ")", x, y, 19, muted); y += 32;
    }

    // ---- подсказка внизу
    float hy = area.height - 118;
    if (hy < y + 6) hy = y + 6;     // на маленьком окне не наезжать на текст
    r.Text("ЛКМ — выбрать и перетащить объект", x, hy, 17, muted);
    r.Text("Q / E — поднять / опустить объект", x, hy + 22, 17, muted);
    r.Text("ПКМ — вращать камеру, колесо — зум", x, hy + 44, 17, muted);
    r.Text("Средняя кнопка — сдвиг камеры", x, hy + 66, 17, muted);
    DrawFPS((int)x, (int)(area.height - 28));
    return a;
}
