#include "TurnAround.h"
#include "Weapons.h"
#include <algorithm>

TurnAround::TurnAround(int activationKey) : activationKey(activationKey) {}

void TurnAround::Execute(Arduino& arduino, const Config& config)
{
    if (!IsKeyPressed(activationKey))
    {
        return;
    }

    const double totalExact = static_cast<double>(config.GetTurnAroundDistance());

    // Dividing the total distance into 8 chunks with a floating-point accumulator.
    // Naive approach (cast each chunk separately) would lose fractional units on
    // every iteration and accumulate drift across repeated presses.
    // Here we round the running sum instead — error stays below 1 unit total.
    constexpr int NUM_CHUNKS = 8;
    double accumulated = 0.0;
    int    totalSent = 0;

    for (int i = 0; i < NUM_CHUNKS; i++)
    {
        accumulated += totalExact / NUM_CHUNKS;

        // Difference between where we should be and where we actually are.
        const int chunk = static_cast<int>(std::round(accumulated)) - totalSent;
        totalSent += chunk;

        // Mouse.move() on Arduino silently clamps input to -127..127,
        // so large chunks are broken into 127-unit steps.
        int remaining = chunk;
        while (remaining > 0)
        {
            const int step = std::min(remaining, 127);
            arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + std::to_string(step) + ",0,1");
            sleep_for(milliseconds(1));
            remaining -= step;
        }
    }
}