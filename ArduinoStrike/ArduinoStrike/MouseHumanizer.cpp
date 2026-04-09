#include "MouseHumanizer.h"
#include "Logger.h"

// ──────────────────────────────────────────────
//  Приватные вспомогательные методы
// ──────────────────────────────────────────────

float MouseHumanizer::EaseInOut(float t)
{
    // Классическая smoothstep: медленно → быстро → медленно
    return t * t * (3.0f - 2.0f * t);
}

float MouseHumanizer::BezierX(float cx, float tx, float t)
{
    // P(t) = (1-t)^2 * 0  +  2*(1-t)*t * cx  +  t^2 * tx
    float inv = 1.0f - t;
    return 2.0f * inv * t * cx + t * t * tx;
}

float MouseHumanizer::BezierY(float cy, float ty, float t)
{
    float inv = 1.0f - t;
    return 2.0f * inv * t * cy + t * t * ty;
}

float MouseHumanizer::RandFloat(float lo, float hi)
{
    // thread_local — каждый поток имеет свой генератор, нет гонок
    thread_local std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(lo, hi);
    return dist(rng);
}

// ──────────────────────────────────────────────
//  Публичные методы
// ──────────────────────────────────────────────

std::vector<MouseMovement> MouseHumanizer::GeneratePath(int dx, int dy, const Options& opts)
{
    std::vector<MouseMovement> path;

    if (opts.steps <= 0 || (dx == 0 && dy == 0))
        return path;

    path.reserve(opts.steps);

    // Контрольная точка кривой Безье — смещена перпендикулярно направлению
    // Направление перпендикуляра: (-dy, dx), нормированное
    float length = std::sqrt(static_cast<float>(dx * dx + dy * dy));
    float perpX = (length > 0.0f) ? (-static_cast<float>(dy) / length) : 0.0f;
    float perpY = (length > 0.0f) ? (static_cast<float>(dx) / length) : 0.0f;

    // Величина отклонения контрольной точки
    float curveDist = length * opts.curve * RandFloat(0.4f, 1.0f);

    // Знак отклонения — случайный (дуга влево или вправо)
    float sign = (RandFloat(0.0f, 1.0f) > 0.5f) ? 1.0f : -1.0f;

    float cx = static_cast<float>(dx) * 0.5f + perpX * curveDist * sign;
    float cy = static_cast<float>(dy) * 0.5f + perpY * curveDist * sign;

    float prevX = 0.0f, prevY = 0.0f;

    for (int i = 1; i <= opts.steps; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(opts.steps);
        float tEase = EaseInOut(t);

        // Позиция на кривой
        float curX = BezierX(cx, static_cast<float>(dx), tEase);
        float curY = BezierY(cy, static_cast<float>(dy), tEase);

        // Добавляем небольшой случайный шум (масштабируется по длине отрезка)
        float noiseScale = length * opts.noise * 0.05f;
        curX += RandFloat(-noiseScale, noiseScale);
        curY += RandFloat(-noiseScale, noiseScale);

        // Дельта относительно предыдущего шага
        int stepDx = static_cast<int>(std::round(curX - prevX));
        int stepDy = static_cast<int>(std::round(curY - prevY));

        // Задержка с вариацией скорости
        float delayVar = opts.base_delay * opts.speed_var;
        int delay = static_cast<int>(opts.base_delay + RandFloat(-delayVar, delayVar));
        delay = std::max(1, delay);

        if (stepDx != 0 || stepDy != 0)
        {
            path.push_back({ stepDx, stepDy, delay });
        }

        prevX = curX;
        prevY = curY;
    }

    // Корректирующий шаг: компенсируем накопленную ошибку округления
    int totalDx = 0, totalDy = 0;
    for (const auto& m : path) { totalDx += m.dx; totalDy += m.dy; }

    int errX = dx - totalDx;
    int errY = dy - totalDy;

    if ((errX != 0 || errY != 0) && !path.empty())
    {
        path.back().dx += errX;
        path.back().dy += errY;
    }

    return path;
}

void MouseHumanizer::SendPath(Arduino& arduino, const std::vector<MouseMovement>& path)
{
    for (const auto& move : path)
    {
        arduino.WriteMessage(
            "MOUSE_LEFT_HOLDED:" +
            std::to_string(move.dx) + "," +
            std::to_string(move.dy) + "," +
            std::to_string(move.delay_ms)
        );
        sleep_for(milliseconds(move.delay_ms));
    }
}

void MouseHumanizer::MoveTo(Arduino& arduino, int dx, int dy, const Options& opts)
{
    auto path = GeneratePath(dx, dy, opts);
    Logger::LogMessage(
        "MouseHumanizer: moving (" + std::to_string(dx) + "," + std::to_string(dy) +
        ") in " + std::to_string(path.size()) + " steps"
    );
    SendPath(arduino, path);
}