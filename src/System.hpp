#pragma once

#include "ECS.hpp"
#include "Component.hpp"
#include <set>

class BaseSystem {
public:
    BaseSystem() {}
    BaseSystem(std::set<uint32_t>& req) : requiredComponents(req) {}
    BaseSystem(std::set<uint32_t>&& req) : requiredComponents(std::move(req)) {}

    virtual void tick(double timestep) = 0;

    template <typename Component>
    void addRequirement() {
        static_assert(std::is_base_of<ECSComponent<Component>, Component>::value, "Provided template argument must be derived of ECSComponent.");
        requiredComponents.insert(Component::ID);
    }

    void setAdmin(ECS* admin);
    void setRequirements(std::set<uint32_t>& req);
    void setRequirements(std::set<uint32_t>&& req);

private:
    template <typename Component>
    std::vector<Component*> getComponents() {
        static_assert(std::is_base_of<ECSComponent<Component>, Component>::value, "Provided template argument must be derived from ECSComponent");
        return admin->query<Component>(requiredComponents);
    }

    ECS* admin;
    std::set<uint32_t> requiredComponents;
};