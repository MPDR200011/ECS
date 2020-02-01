#pragma once

#include <set>
#include <unordered_map>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"

typedef Entity* EntityHandle;

class ECSPool {
public:
    ECSPool() {}
    ~ECSPool();

    EntityHandle createEntity();

    void removeEntity(EntityHandle entityID);

    template <typename Component>
    void addComponent(EntityHandle entity, const Component& comp) {
        VALID_COMP(Component);

        ECSComponentCreateFunction createfn =
            BaseECSComponent::getTypeCreateFunction(Component::ID);
        ComponentPool& pool = components[Component::ID];

        entity->components[Component::ID] = createfn(pool, entity, &comp);
    }

    template <typename... Components>
    EntityHandle createEntity(const Components&... comps) {
        static_assert(
            (std::is_base_of_v<ECSComponent<Components>, Components> && ...),
            "Provided template argument must be derived from ECSComponent.");

        EntityHandle entity = createEntity();

        (addComponent(entity, comps), ...);

        return entity;
    }

    template <typename Component>
    bool removeComponent(EntityHandle entity) {
        VALID_COMP(Component);

        auto search = entity->components.find(Component::ID);
        if (search != entity->components.end()) {
            deleteComponent(Component::ID, search->second.first);
        }

        return entity->components.erase(Component::ID) > 0;
    }

    template <typename Component>
    Component* getComponent(EntityHandle entity) {
        VALID_COMP(Component);

        auto search = entity->components.find(Component::ID);
        if (search != entity->components.end()) {
            return (Component*)search->second.second;
        }

        return nullptr;
    }

    const std::vector<Entity*> query(std::set<uint32_t>& req) {
        std::vector<Entity*> res;

        if (req.size() == 0) {
            return res;
        }

        uint32_t firstComp = *req.begin();

        ComponentPool& componentPool = components[firstComp];
        if (req.size() == 0) {
            for (BaseECSComponent* comp : componentPool) {
                res.push_back(comp->entity);
            }

            return res;
        }

        for (BaseECSComponent* comp : componentPool) {
            bool valid = true;
            for (uint32_t id : req) {
                if (!comp->entity->components.count(id)) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                res.push_back(comp->entity);
            }
        }

        return res;
    }

    template <typename Component>
    Component* getSingleton() {
        VALID_COMP(Component);
        return (Component*)singletons.at(Component::ID);
    }

    template <typename Component>
    Component* createSingleton(const Component& component) {
        VALID_COMP(Component);
        if (singletons.count(Component::ID) > 0) {
            return (Component*)singletons[Component::ID];
        }

        BaseECSComponent* comp = new Component(*(Component*)&component);

        singletons[Component::ID] = comp;

        return (Component*)comp;
    }

    template <typename Component>
    void removeSingleton() {
        singletons.erase(Component::ID);
    }

private:
    void deleteComponent(uint32_t compoonentType, size_t index);

    std::unordered_map<uint32_t, BaseECSComponent*> singletons;
    std::unordered_map<uint32_t, ComponentPool> components;
    std::vector<Entity*> entities;
};
