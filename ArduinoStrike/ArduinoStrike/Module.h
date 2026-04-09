#pragma once

#include "Config.h"
#include "Arduino.h"

// Kind of module, used by ModuleManager to decide how to schedule it.
enum class ModuleKind
{
    Regular,
    System
};

class Module
{
public:
    virtual ~Module() = default;

    // Virtual front method: by default delegates to Execute(), but
    // can be overridden (e.g. ToggleableModule / HoldableModule).
    virtual void Process(Arduino& arduino, const Config& config)
    {
        Execute(arduino, config);
    }

    // By default any module is treated as a regular one.
    // System-like modules override this to return ModuleKind::System.
    virtual ModuleKind GetKind() const noexcept
    {
        return ModuleKind::Regular;
    }

protected:
    // Core module behaviour implementation.
    // Default implementation is empty - modules that override Process() don't need to implement this.
    virtual void Execute(Arduino& arduino, const Config& config)
    {
        // Empty by default - override Process() or Execute() as needed
    }
};