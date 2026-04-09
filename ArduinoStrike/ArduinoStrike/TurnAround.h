#pragma once

#include "Module.h"

class TurnAround : public Module
{
public:
    explicit TurnAround(int activationKey);

protected:
    void Execute(Arduino& arduino, const Config& config) override;

private:
    int activationKey;
};