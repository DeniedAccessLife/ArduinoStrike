#pragma once

#include "SystemModule.h"

class ModuleManager
{
public:
    template <typename T, typename... Args>
    void AddModule(const string& name, Args&&... args)
    {
        static_assert(is_base_of_v<Module, T>, "Module type must inherit from Module base class");

        if (modules_map.contains(name))
        {
            throw invalid_argument("Module " + name + " already registered");
        }

        auto module = make_unique<T>(forward<Args>(args)...);

        if constexpr (is_base_of_v<SystemModule, T>)
        {
            system_modules.push_back(move(module));
            modules_map[name] = system_modules.back().get();
        }
        else
        {
            modules_map[name] = module.get();
            regular_modules.push_back(move(module));
        }
    }

    template <typename T>
    T* GetModule(const string& name) noexcept
    {
        auto iterator = modules_map.find(name);
        return iterator != modules_map.end() ? static_cast<T*>(iterator->second) : nullptr;
    }

    void ProcessModules(Arduino& arduino, const Config& config) noexcept
    {
        for (auto& module : regular_modules)
        {
            module->Process(arduino, config);
        }
    }

private:
    unordered_map<string, Module*> modules_map;
    vector<unique_ptr<Module>> regular_modules;
    vector<unique_ptr<SystemModule>> system_modules;
};