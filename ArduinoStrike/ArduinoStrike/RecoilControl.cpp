#include "FastReload.h"
#include "RecoilControl.h"

RecoilControl::RecoilControl(ModuleManager& manager) : manager(manager), weapon(OFF) {}

void RecoilControl::Execute(Arduino& arduino, const Config& config)
{
    weapon = GetWeaponState(weapon, config);
    if (weapon == OFF) return;

    double obs = 0.75 / config.GetZoomSensitivity();
    double modifier = CalculateModifier(config, obs);

    WeaponData data = GetWeaponData(weapon, modifier);

    if (!ValidateWeaponData(data))
    {
        weapon = OFF;
        return;
    }

    ProcessRecoilData(arduino, config, data);
}

double RecoilControl::CalculateModifier(const Config& config, double obs)
{
    double modifier = 2.52 / config.GetSensitivity();
    return IsKeyHolded(VK_RBUTTON) ? modifier * obs : modifier;
}

bool RecoilControl::ValidateWeaponData(const WeaponData& data)
{
    return data.x.size() == data.y.size() && data.x.size() == data.delay.size();
}

void RecoilControl::ProcessRecoilData(Arduino& arduino, const Config& config, const WeaponData& data)
{
    for (size_t i = 0; i < data.x.size(); i++)
    {
        if (!IsKeyHolded(VK_LBUTTON) || (config.GetConfirmationKey() != 0 && !IsKeyHolded(config.GetConfirmationKey())))
        {
            return;
        }

        arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + to_string(data.x[i]) + "," + to_string(data.y[i]) + "," + to_string(data.delay[i]));
        sleep_for(milliseconds(data.delay[i]));
    }

    arduino.WriteMessage("MOUSE_LEFT_CLICK");

    if (auto* fastReload = manager.GetModule<FastReload>("FastReload"))
    {
        fastReload->SetCurrentWeapon(weapon);
        fastReload->Process(arduino, config);
    }
}