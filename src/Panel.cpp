#include "Panel.h"
#include <cstdio>
#include <algorithm>

Rectangle Panel::Area() const
{
    return { GetScreenWidth() - width, 0, width, (float)GetScreenHeight() };
}

bool Panel::Button(const Renderer& r, Rectangle rc, const std::string& label, bool active) const
{
    bool hover = CheckCollisionPointRec(GetMousePosition(), rc);
    Color bg = active ? theme.accent : (hover ? theme.buttonHover : theme.button);
    DrawRectangleRounded(rc, theme.roundness, 6, bg);
    DrawRectangleRoundedLinesEx(rc, theme.roundness, 6, 1.0f, active ? theme.accent : theme.buttonBorder);
    float tw = r.TextWidth(label, theme.textSize);
    r.Text(label, rc.x + (rc.width - tw) / 2, rc.y + (rc.height - theme.textSize) / 2, theme.textSize, active ? theme.textOnAccent : theme.text);
    return hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

float Panel::DrawCollisionList(const Renderer& r, const PanelState& s, float x, float y, float w)
{
    const float rowH = theme.listRowHeight;
    const float T = theme.textSize - 1;
    const int n = (int)s.liveCollisions.size();
    const int visible = std::min(n, theme.listVisibleRows);

    Rectangle box = { x, y, w, visible * rowH + 8 };       // окно списка
    const float viewH = box.height - 8;                    // видимая высота строк
    const float maxScroll = std::max(0.0f, n * rowH - viewH);

    // полоса прокрутки справа: дорожка и ползунок
    const float track = viewH;
    const float thumbH = maxScroll > 0 ? std::max(24.0f, track * viewH / (n * rowH)) : track;
    Rectangle trackRc = { box.x + box.width - 10, box.y + 4, 6, track };

    // ввод: колесо мыши над списком и перетаскивание ползунка
    Vector2 m = GetMousePosition();
    if (CheckCollisionPointRec(m, box))
        listScroll -= GetMouseWheelMove() * rowH * 3;

    float thumbY = trackRc.y + (maxScroll > 0 ? (track - thumbH) * (listScroll / maxScroll) : 0);
    Rectangle thumbHit = { trackRc.x - 4, thumbY, trackRc.width + 8, thumbH };   // чуть шире, чтобы легче попасть
    if (maxScroll > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(m, thumbHit)) {
        draggingThumb = true;
        dragStartMouseY = m.y;
        dragStartScroll = listScroll;
    }
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) draggingThumb = false;
    if (draggingThumb && track > thumbH)
        listScroll = dragStartScroll + (m.y - dragStartMouseY) * maxScroll / (track - thumbH);

    listScroll = std::clamp(listScroll, 0.0f, maxScroll);  // список мог стать короче
    thumbY = trackRc.y + (maxScroll > 0 ? (track - thumbH) * (listScroll / maxScroll) : 0);

    // отрисовка: фон, затем только видимые строки
    DrawRectangleRounded(box, 0.06f, 6, theme.listBackground);
    BeginScissorMode((int)box.x, (int)(box.y + 4), (int)box.width, (int)viewH);
    int first = (int)(listScroll / rowH);
    for (int i = first; i < n && i <= first + visible; i++) {
        float ry = box.y + 4 + i * rowH - listScroll;
        const CollisionPair& p = s.liveCollisions[i];
        r.Text(std::to_string(i + 1) + ".", x + 10, ry + 2, T, theme.textMuted);
        r.Text(s.scene->objects[p.a].name + " – " + s.scene->objects[p.b].name, x + 56, ry + 2, T, theme.danger);
    }
    EndScissorMode();

    if (maxScroll > 0) {
        DrawRectangleRounded(trackRc, 1.0f, 4, theme.scrollbar);
        DrawRectangleRounded({ trackRc.x, thumbY, trackRc.width, thumbH }, 1.0f, 4,
                             draggingThumb ? theme.textMuted : theme.scrollbarThumb);
    }
    return box.y + box.height + 6;
}

static std::string Fmt(const char* f, double v) { char b[64]; snprintf(b, sizeof b, f, v); return b; }
static std::string FmtLL(long long v) { return std::to_string(v); }

PanelActions Panel::Draw(const Renderer& r, const PanelState& s)
{
    PanelActions a;
    Rectangle area = Area();
    DrawRectangleRec(area, theme.panel);
    DrawLine((int)area.x, 0, (int)area.x, (int)area.height, theme.panelBorder);

    const Color ink = theme.text, muted = theme.textMuted;
    const float S = theme.sectionSize, T = theme.textSize, H = theme.hintSize;
    const float bh = theme.buttonHeight, sbh = theme.smallButtonHeight, gap = theme.buttonGap;
    float x = area.x + theme.padding, y = 16, w = width - 2 * theme.padding;

    r.Text("Sketchpad 3D", x, y, theme.titleSize, ink); y += 40;

    //алгоритмы
    r.Text("Алгоритм поиска коллизий", x, y, S, muted); y += 28;
    for (int i = 0; i < (int)s.algorithms.size(); i++) {
        if (Button(r, { x, y, w, bh }, s.algorithms[i], i == s.activeAlgorithm)) a.algorithmClicked = i;
        y += bh + gap;
    }

    // результаты замера 
    y += 6;
    r.Text("Последний замер", x, y, S, muted); y += 28;
    if (!s.hasMeasurement) {
        r.Text("Нажмите на алгоритм,", x, y, S, ink); y += 24;
        r.Text("чтобы выполнить проверку", x, y, S, ink); y += 34;
    } else {
        r.Text("Время:  " + Fmt("%.4f", s.measured.timeMs) + " мс", x, y, T, ink); y += 26;
        r.Text("Тестов AABB:  " + FmtLL(s.measured.aabbTests), x, y, T, ink); y += 26;
        r.Text("Пар-кандидатов:  " + FmtLL(s.measured.candidatePairs), x, y, T, ink); y += 34;
    }

    // текущее состояние (обновляется каждый кадр)
    r.Text("Сейчас", x, y, S, muted); y += 28;
    r.Text("Объектов:  " + std::to_string(s.objectCount), x, y, T, ink); y += 26;
    Color hitColor = s.liveCollisions.empty() ? ink : theme.danger;
    r.Text("Коллизий:  " + std::to_string(s.liveCollisions.size()), x, y, T, hitColor);
    if (!s.liveCollisions.empty()) {
        // кнопка справа от счётчика раскрывает и скрывает список
        const float bw = 110;
        if (Button(r, { x + w - bw, y - 4, bw, 28 }, listOpen ? "Скрыть" : "Список", listOpen))
            listOpen = !listOpen;
    }
    y += 32;
    if (listOpen && !s.liveCollisions.empty() && s.scene)
        y = DrawCollisionList(r, s, x, y, w);
    y += 10;

    // сцена
    r.Text("Сцена", x, y, S, muted); y += 28;
    if (Button(r, { x, y, w, sbh }, "Тестовая сцена A–F", false)) a.loadTestScene = true;
    y += sbh + gap;
    float hw = (w - gap) / 2;
    if (Button(r, { x, y, hw, sbh }, "100 объектов", false)) a.randomCount = 100;
    if (Button(r, { x + hw + gap, y, hw, sbh }, "1000 объектов", false)) a.randomCount = 1000;
    y += sbh + gap;
    if (Button(r, { x, y, w, sbh }, s.showAABB ? "Скрыть AABB" : "Показать AABB", false)) a.toggleAABB = true;
    y += 50;

    // выбранный объект
    if (s.scene && s.selected >= 0) {
        const SceneObject& o = s.scene->objects[s.selected];
        r.Text("Выбран: " + o.name, x, y, T, ink); y += 26;
        r.Text("(" + Fmt("%.1f", o.position.x) + "; " + Fmt("%.1f", o.position.y) + "; " + Fmt("%.1f", o.position.z) + ")", x, y, S, muted); y += 32;
    }

    // подсказка внизу
    float hy = area.height - 118;
    if (hy < y + 6) hy = y + 6;     // на маленьком окне не наезжать на текст
    r.Text("ЛКМ — выбрать и перетащить объект", x, hy, H, muted);
    r.Text("Q / E — поднять / опустить объект", x, hy + 22, H, muted);
    r.Text("ПКМ — вращать камеру, колесо — зум", x, hy + 44, H, muted);
    r.Text("Средняя кнопка — сдвиг камеры", x, hy + 66, H, muted);
    float fy = area.height - 28;
    if (fy < hy + 90) fy = hy + 90;   // FPS не наезжает на подсказки
    r.Text(std::to_string(GetFPS()) + " FPS", x, fy, H, muted);
    return a;
}
