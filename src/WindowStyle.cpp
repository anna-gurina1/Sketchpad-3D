#include "WindowStyle.h"

#ifdef _WIN32

extern "C" __declspec(dllimport) long __stdcall
DwmSetWindowAttribute(void *hwnd, unsigned long attribute, const void *value, unsigned long size);

static unsigned long ToColorRef(Color c)
{
    return (unsigned long)c.r | ((unsigned long)c.g << 8) | ((unsigned long)c.b << 16);
}

void ApplyTitleBarStyle(Color background, Color text)
{
    void *hwnd = GetWindowHandle();
    int dark = 1;

    // тёмный режим заголовка
    if (DwmSetWindowAttribute(hwnd, 20, &dark, sizeof dark) != 0)
        DwmSetWindowAttribute(hwnd, 19, &dark, sizeof dark);

    // точный цвет полосы и текста
    unsigned long bg = ToColorRef(background), fg = ToColorRef(text);
    DwmSetWindowAttribute(hwnd, 35, &bg, sizeof bg);
    DwmSetWindowAttribute(hwnd, 36, &fg, sizeof fg);
}
#else
void ApplyTitleBarStyle(Color, Color) {}
#endif
