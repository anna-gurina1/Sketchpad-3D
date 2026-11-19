#pragma once
#include "raylib.h"

// Внешний вид приложения: все цвета и размеры интерфейса
struct Theme {
    //3D-сцена
    Color backgroundTop    = {  30,  34,  42, 255 };   // фон сверху (градиент)
    Color backgroundBottom = {  17,  19,  24, 255 };   // фон снизу
    Color grid             = {  48,  54,  64, 255 };   // клетки на полу
    Color objectEdges      = {   0,   0,   0,  70 };   // рёбра треугольников на объектах
    Color aabb             = { 160, 168, 180, 140 };   // каркас AABB
    Color aabbHit          = { 255, 120, 110, 255 };   // каркас AABB при коллизии
    Color collision        = { 235,  77,  75, 255 };   // подсветка объектов в коллизии
    Color selection        = { 255, 209, 102, 255 };   // рамка выбранного объекта
    Color label            = { 236, 239, 244, 255 };   // подписи A, B, C над объектами
    Color axisX = { 235,  87,  87, 255 };
    Color axisY = {  94, 200, 120, 255 };
    Color axisZ = {  90, 150, 255, 255 };

    //заголовок окна Windows (полоса сверху)
    Color titleBar         = {  24,  27,  33, 255 };   // фон полосы (точный цвет — только Windows 11)
    Color titleText        = { 230, 233, 238, 255 };   // текст названия окна

    // панель
    Color panel            = {  24,  27,  33, 255 };   // фон панели
    Color panelBorder      = {  44,  49,  58, 255 };   // линия слева от панели
    Color text             = { 230, 233, 238, 255 };   // основной текст
    Color textMuted        = { 139, 147, 161, 255 };   // заголовки разделов, подсказки
    Color textOnAccent     = { 255, 255, 255, 255 };   // текст на активной кнопке
    Color accent           = {  79, 140, 255, 255 };   // активная кнопка
    Color button           = {  36,  40,  48, 255 };
    Color buttonHover      = {  47,  52,  62, 255 };
    Color buttonBorder     = {  58,  64,  75, 255 };
    Color danger           = { 255,  99,  92, 255 };   // текст «Коллизий: N» и список пар
    Color listBackground   = {  31,  35,  42, 255 };   // фон раскрывающегося списка коллизий
    Color scrollbar        = {  44,  49,  58, 255 };   // дорожка полосы прокрутки
    Color scrollbarThumb   = {  96, 104, 118, 255 };   // ползунок полосы прокрутки

    //размеры
    float panelWidth   = 360.0f;
    float padding      = 18.0f;    // отступ от края панели
    float buttonHeight = 38.0f;    // кнопки алгоритмов
    float smallButtonHeight = 36.0f;  // кнопки сцены
    float buttonGap    = 8.0f;     // расстояние между кнопками
    float roundness    = 0.25f;    // скругление кнопок
    float listRowHeight   = 24.0f; // высота строки в списке коллизий
    int   listVisibleRows = 8;     // сколько строк списка видно без прокрутки

    // шрифт
    float titleSize   = 28.0f;     // «Sketchpad 3D»
    float sectionSize = 19.0f;     // заголовки разделов
    float textSize    = 20.0f;     // обычный текст
    float hintSize    = 17.0f;     // подсказки внизу
    float labelSize   = 22.0f;     // подписи над объектами
};

inline Theme theme; 