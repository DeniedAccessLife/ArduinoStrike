#pragma once

#include "Module.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

class ModuleManager
{
public:
    template <typename T, typename... Args>
    T& AddModule(const std::string& name, Args&&... args)
    {
        static_assert(std::is_base_of_v<Module, T>, "Module type must inherit from Module base class");

        if (modules_by_name.contains(name))
        {
            throw std::invalid_argument("Module " + name + " already registered");
        }

        const std::type_index type = std::type_index(typeid(T));
        if (modules_by_type.contains(type))
        {
            throw std::invalid_argument("Module type already registered");
        }

        auto module = std::make_unique<T>(std::forward<Args>(args)...);
        T* const raw = module.get();

        modules_by_name[name] = raw;
        modules_by_type[type] = raw;
        regular_modules.push_back(std::move(module));
        return *raw;
    }

    template <typename T>
    T* GetModule(const std::string& name) noexcept
    {
        auto iterator = modules_by_name.find(name);
        return iterator != modules_by_name.end() ? static_cast<T*>(iterator->second) : nullptr;
    }

    template <typename T>
    T* GetModule() noexcept
    {
        auto iterator = modules_by_type.find(std::type_index(typeid(T)));
        return iterator != modules_by_type.end() ? static_cast<T*>(iterator->second) : nullptr;
    }

    void ProcessModules(Arduino& arduino, const Config& config) noexcept
    {
        for (auto& module : regular_modules)
        {
            module->Process(arduino, config);
        }
    }

private:
    std::unordered_map<std::string, Module*> modules_by_name;
    std::unordered_map<std::type_index, Module*> modules_by_type;
    std::vector<std::unique_ptr<Module>> regular_modules;
};