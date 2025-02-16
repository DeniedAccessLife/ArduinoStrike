#pragma once

#include "Config.h"
#include "Arduino.h"

class Module
{
public:
    virtual ~Module() = default;

    virtual void Process(Arduino& arduino, const Config& config) = 0;
};