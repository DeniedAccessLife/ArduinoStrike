#include "FastReload.h"
#include "RecoilControl.h"
#include "MouseHumanizer.h"

// RecoilControl is responsible for applying weapon?specific recoil compensation
// using precomputed spray patterns from the config. The idea is similar to
// AutoAccept: the PC decides *what* to do, the Arduino only receives simple
// high?level mouse commands (relative moves + delays).
RecoilControl::RecoilControl(ModuleManager& manager) : manager(manager), weapon(OFF) {}

void RecoilControl::Execute(Arduino& arduino, const Config& config)
{
    // Determine which weapon is currently active (or OFF if nothing matches
    // the configured hotkeys). State is stored in the object so it can persist
    // across multiple Execute calls.
    weapon = GetWeaponState(weapon, config);
    if (weapon == OFF) return;

    // OBS zoom sensitivity factor:
    //  - base value 0.75 is the default scoped sensitivity multiplier in CS2
    //  - we normalise it by the user?provided zoom sensitivity so that RCS
    //    behaves consistently in both scoped and unscoped modes.
    double obs = 0.75 / config.GetZoomSensitivity();

    // Global mouse?sensitivity scale. Higher in?game sensitivity means we need
    // proportionally smaller Arduino steps to achieve the same visual recoil
    // compensation on screen.
    double modifier = CalculateModifier(config, obs);

    // Fetch precomputed weapon pattern (x/y offsets + delays between bullets)
    // adjusted by the global modifier and per?weapon config multipliers.
    WeaponData data = GetWeaponData(weapon, modifier, config);

    // Sanity?check that all per?shot arrays have identical length before
    // starting to send mouse moves. If anything is misconfigured, abort and
    // reset state to OFF.
    if (!ValidateWeaponData(data))
    {
        weapon = OFF;
        return;
    }

    // Apply the pattern step?by?step, adding a small amount of randomness
    // to keep the recoil control from looking like a perfectly scripted macro.
    ProcessRecoilData(arduino, config, data);
}

double RecoilControl::CalculateModifier(const Config& config, double obs)
{
    // Base factor converts in?game sensitivity into Arduino "units".
    // This keeps the recoil pattern roughly correct even if the user changes
    // their sensitivity without recalibrating all weapons.
    double modifier = 2.52 / config.GetSensitivity();

    // When the right mouse button is held (usually scoped / ADS mode), we
    // additionally scale by the OBS factor so that recoil compensation stays
    // proportional while zoomed?in.
    return IsKeyHolded(VK_RBUTTON) ? modifier * obs : modifier;
}

bool RecoilControl::ValidateWeaponData(const WeaponData& data)
{
    // All axes + delays must have matching lengths:
    //   data.x[i]  – horizontal compensation for shot i
    //   data.y[i]  – vertical   compensation for shot i
    //   data.delay[i] – milliseconds before the next shot
    return data.x.size() == data.y.size() && data.x.size() == data.delay.size();
}

void RecoilControl::ProcessRecoilData(Arduino& arduino, const Config& config, const WeaponData& data)
{
    // Thread?local RNG instance used to humanise the pattern. This matches
    // the approach used in AutoAccept: randomness is cheap and completely
    // local to this function.
    thread_local std::mt19937 rng{ std::random_device{}() };

    // Small jitter simulates natural hand tremor on both axes.
    // ±1 unit is barely noticeable for precision, but enough to prevent any
    // two sprays from being bit?perfect copies of each other.
    std::uniform_int_distribution<int> jitter(-1, 1);

    // Slight delay variation mimics inconsistent human reaction time between
    // bullets. Bounds are tight so RCS timing stays in sync with the weapon's
    // real fire rate while still looking organic.
    std::uniform_real_distribution<float> delayVar(0.92f, 1.08f); // ±8%

    for (size_t i = 0; i < data.x.size(); i++)
    {
        // Abort immediately if:
        //  - left mouse button is released (player stopped firing); or
        //  - a confirmation key is configured but is no longer held.
        // This makes the module fully responsive to player input.
        if (!IsKeyHolded(VK_LBUTTON) || (config.GetConfirmationKey() != 0 && !IsKeyHolded(config.GetConfirmationKey())))
        {
            return;
        }

        // Base recoil compensation for this bullet, plus tiny per?axis jitter.
        const int rx = static_cast<int>(data.x[i]) + jitter(rng);
        const int ry = static_cast<int>(data.y[i]) + jitter(rng);

        // Base delay between bullets, scaled by a small random factor.
        const int rd = static_cast<int>(data.delay[i] * delayVar(rng));

        // Send a relative mouse movement that is held for `rd` milliseconds.
        // Protocol is shared with AutoAccept:
        //   "MOUSE_LEFT_HOLDED:dx,dy,duration_ms"
        arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + to_string(rx) + "," + to_string(ry) + "," + to_string(rd));

        // Wait for the duration of the move so that the next step lines up
        // with when the Arduino finishes applying this delta on the HID side.
        sleep_for(milliseconds(rd));
    }

    // After the pattern is fully consumed, send a final left?click to help
    // with burst control and to unify behaviour with other mouse modules.
    arduino.WriteMessage("MOUSE_LEFT_CLICK");

    // Optionally invoke FastReload so that any weapon?specific reload timing
    // logic can react to the weapon that just finished spraying.
    if (auto* fastReload = manager.GetModule<FastReload>())
    {
        fastReload->SetCurrentWeapon(weapon);
        fastReload->Process(arduino, config);
    }
}