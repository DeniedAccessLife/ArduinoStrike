#include "RapidFire.h"

RapidFire::RapidFire(int activationKey) : HoldableModule(activationKey) {}

void RapidFire::OnKeyHold(Arduino& arduino, const Config& config)
{
    if (config.GetRapidFire() != 0)
    {
        arduino.WriteMessage("MOUSE_MIDDLE_HOLDED");
    }
}