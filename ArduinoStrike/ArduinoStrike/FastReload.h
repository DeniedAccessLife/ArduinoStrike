#pragma once

#include "Arduino.h"
#include "Weapons.h"
#include <unordered_map>

class FastReload
{
public:
    FastReload();
    void Process(const Arduino& arduino, const Weapon weapon) const;

private:
    unordered_map<Weapon, int> times;
};