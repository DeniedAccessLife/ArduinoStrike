#pragma once

#include "Config.h"
#include "ConfigSchema.h"

class ConfigWizard
{
public:
    explicit ConfigWizard(Config& cfg);

    void Run();

private:
    Config& config;

    bool OfferDefaultConfig();
    void ConfigureFeatures();
    void ConfigureSettings();
    void ConfigureKeys();
};