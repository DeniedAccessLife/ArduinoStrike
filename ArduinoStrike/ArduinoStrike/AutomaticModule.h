#pragma once

#include "Module.h"

class AutomaticModule : public Module
{
public:
    void Process(Arduino& arduino, const Config& config) override
    {
        Execute(arduino, config);
    }

protected:
    virtual void Execute(Arduino& arduino, const Config& config) = 0;
};