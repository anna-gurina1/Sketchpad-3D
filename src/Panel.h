#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "Renderer.h"
#include "Scene.h"
#include "CollisionDetector.h"
#include "Theme.h"

// Что показать на панели справа
struct PanelState {
    std::vector<std::string> algorithms;   // названия алгоритмов
    int activeAlgorithm = -1;              // ещё ни разу не нажимали
    bool hasMeasurement = false;           // замер есть только после нажатия кнопки
    AlgorithmMetrics measured;             // результаты последнего замера
    int objectCount = 0;
    std::vector<CollisionPair> liveCollisions;   //текущие коллизии (каждый кадр)
    const Scene* scene = nullptr;
    int selected = -1;
    bool showAABB = true;
};

// Что нажали на панели в этом кадре
struct PanelActions {
    int algorithmClicked = -1;
    bool loadTestScene = false;
    int randomCount = 0;      //создать случайную сцену из стольких объектов
    bool toggleAABB = false;
};

class Panel {
public:
    float width = theme.panelWidth;
    Rectangle Area() const;          // прямоугольник панели на экране
    PanelActions Draw(const Renderer& r, const PanelState& s);

private:
    bool Button(const Renderer& r, Rectangle rc, const std::string& label, bool active) const;

    // раскрывающийся список коллизий с прокруткой; возвращает y под списком
    float DrawCollisionList(const Renderer& r, const PanelState& s, float x, float y, float w);
    bool  listOpen = false;        // список раскрыт
    float listScroll = 0.0f;       // насколько прокручен список, в пикселях
    bool  draggingThumb = false;   // тянем ползунок мышью
    float dragStartMouseY = 0.0f;
    float dragStartScroll = 0.0f;
};
