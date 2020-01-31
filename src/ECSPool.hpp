#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "Component.hpp"

typedef Entity* EntityHandle;

class ECSPool {
public:
    ECSPool() {}
    ~ECSPool();

    EntityHandle createEntity();

    void removeEntity(EntityHandle entityID);

    template <typename Component>
    void addComponent(EntityHandle entity, const Component& comp);

    template <typename... Components>
    EntityHandle createEntity(const Components&... comps);

    template <typename Component>
    bool removeComponent(EntityHandle entity);

    template <typename Component>
    Component* getComponent(EntityHandle entity);

    template <typename Component>
    const std::vector<Component*> query(std::set<uint32_t>& req);

    template <typename Component>
    Component* getSingleton();

    template <typename Component>
    Component* createSingleton(Component& component);

    template <typename Component>
    void removeSingleton();

private:
    void deleteComponent(uint32_t compoonentType, size_t index);

    std::unordered_map<uint32_t, BaseECSComponent*> singletons;
    std::unordered_map<uint32_t, ComponentPool> components;
    std::vector<Entity*> entities;
};

template <typename Component>
void ECSPool::addComponent(EntityHandle entity, const Component& comp) {
    VALID_COMP(Component);

    ECSComponentCreateFunction createfn =
        BaseECSComponent::getTypeCreateFunction(Component::ID);
    ComponentPool& pool = components[Component::ID];

    entity->components[Component::ID] = createfn(pool, entity, &comp);
}

template <typename... Components>
EntityHandle ECSPool::createEntity(const Components&... comps) {
    static_assert(
        (std::is_base_of_v<ECSComponent<Components>, Components> && ...),
        "Provided template argument must be derived from ECSComponent.");

    EntityHandle entity = createEntity();

    (addComponent(entity, comps), ...);

    return entity;
}

template <typename Component>
bool ECSPool::removeComponent(EntityHandle entity) {
    VALID_COMP(Component);

    auto pair = entity->components[Component::ID];
    if (pair.second != nullptr) {
        deleteComponent(Component::ID, pair.first);
    }

    return entity->components.erase(Component::ID) > 0;
}

template <typename Component>
Component* ECSPool::getComponent(EntityHandle entity) {
    VALID_COMP(Component);

    EntityComponentsMap& entityComponents = entity->components;
    auto search = entityComponents.find(Component::ID);
    if (search != entityComponents.end()) {
        return search->second.second;
    }

    return nullptr;
}

template <typename Component>
const std::vector<Component*> ECSPool::query(std::set<uint32_t>& req) {
    VALID_COMP(Component);
    std::vector<Component*> res;

    if (req.size() == 0) {
        return res;
    }

    ComponentPool& componentPool = components[Component::ID];
    if (req.size() == 1) {
        for (size_t i = 0; i < componentPool.size(); i++) {
            Component* comp = (Component*)&componentPool[i];
            res.push_back(comp);
        }

        return res;
    }

    for (size_t i = 0; i < componentPool.size(); i++) {
        Component* comp = (Component*)componentPool[i];

        bool valid = true;
        for (uint32_t id : req) {
            if (!comp->entity->components.count(id)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            res.push_back(comp);
        }
    }

    return res;
}

template <typename Component>
Component* ECSPool::getSingleton() {
    VALID_COMP(Component);
    return singletons.at(Component::ID);
}

template <typename Component>
Component* ECSPool::createSingleton(Component& component) {
    VALID_COMP(Component);
    if (singletons.count(Component::ID) > 0) {
        return singletons[Component::ID];
    }

    Component* comp = new Component(*(Component*)&component);

    singletons[Component::ID] = comp;

    return comp;
}

template <typename Component>
void ECSPool::removeSingleton() {
    singletons.erase(Component::ID);
}