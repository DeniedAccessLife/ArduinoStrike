#pragma once

#include "pch.h"
#include "Arduino.h"
#include <vector>
#include <random>
#include <cmath>

struct MouseMovement
{
    int dx;
    int dy;
    int delay_ms;
};

class MouseHumanizer
{
public:
    struct Options
    {
        int   steps = 12;    // Количество шагов пути
        int   base_delay = 8;     // Базовая задержка между шагами (мс)
        float noise = 0.12f; // Амплитуда шума (0.0 — нет, 1.0 — максимум)
        float curve = 0.35f; // Кривизна траектории (0.0 — прямая, 1.0 — сильная дуга)
        float speed_var = 0.20f; // Вариация скорости (±, доля от base_delay)
    };

    // Генерирует список относительных движений по кривой Безье от (0,0) до (dx, dy)
    static std::vector<MouseMovement> GeneratePath(int dx, int dy, const Options& opts = {});

    // Отправляет готовый путь на Arduino
    static void SendPath(Arduino& arduino, const std::vector<MouseMovement>& path);

    // Удобный комбинированный метод: генерирует и отправляет
    static void MoveTo(Arduino& arduino, int dx, int dy, const Options& opts = {});

private:
    // Квадратичный Безье: P0=(0,0), P1=control, P2=(dx,dy)
    static float BezierX(float cx, float tx, float t);
    static float BezierY(float cy, float ty, float t);

    // Плавное ускорение/замедление (ease-in-out)
    static float EaseInOut(float t);

    // Псевдослучайное float в диапазоне [lo, hi]
    static float RandFloat(float lo, float hi);
};