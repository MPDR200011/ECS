#pragma once

#include "ECSPool.hpp"
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
        VALID_COMP(Component);
        requiredComponents.insert(Component::ID);
    }

    void setPool(ECSPool* admin);
    void setRequirements(std::set<uint32_t>& req);
    void setRequirements(std::set<uint32_t>&& req);

protected:
    template <typename Component>
    std::vector<Component*> componentIterator();

private:
    ECSPool* pool;
    std::set<uint32_t> requiredComponents;
};

template <typename Component>
std::vector<Component*> BaseSystem::componentIterator() {
    VALID_COMP(Component);
    return pool->query<Component>(requiredComponents);
}