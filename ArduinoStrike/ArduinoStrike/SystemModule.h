#pragma once

#include "Module.h"

// Thin semantic tag over Module to mark a module as "system" one.
// Behaviour is still defined by Execute(), but ModuleManager will
// treat these modules differently via GetKind().
class SystemModule : public Module
{
public:
    ModuleKind GetKind() const noexcept override
    {
        return ModuleKind::System;
    }
};