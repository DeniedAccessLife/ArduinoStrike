#include "Bhop.h"

Bhop::Bhop(int activationKey) : HoldableModule(activationKey) {}

void Bhop::OnKeyHold(Arduino& arduino, const Config& config)
{
    if (config.GetBhop() != 0)
    {
        arduino.WriteMessage("SPACE_BUTTON_HOLDED");
    }
}