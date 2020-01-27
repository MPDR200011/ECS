#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include "Component.hpp"

struct Entity {
private:
    size_t index;
    std::unordered_map<uint32_t, size_t> components;

    friend class ECS;
};

typedef Entity* EntityHandle;

class ECS {
public:
    ECS() {}
    ~ECS();

    EntityHandle createEntity();
    void removeEntity(EntityHandle entityID);

    template<typename Component>
    void addComponent(EntityHandle entity, Component* comp) {
        static_assert(std::is_base_of<ECSComponent<Component>, Component>::value, "Given component must be a valid component type.");

        ECSComponentCreateFunction createfn = BaseECSComponent::getTypeCreateFunction(Component::ID);
        std::vector<uint8_t>& memory = components[Component::ID]; 

        entity->components[Component::ID] = createfn(memory, entity, comp);
    }

    template<typename Component>
    bool removeComponent(EntityHandle entity) {
        static_assert(std::is_base_of<ECSComponent<Component>, Component>::value, "Given component must be a valid component type.");
        
        return entity->components.erase(Component::ID) > 0;
    }

    template<typename Component>
    Component* getComponent(EntityHandle entity) {
        static_assert(std::is_base_of<ECSComponent<Component>, Component>::value, "Given component must be a valid component type.");
        
        std::unordered_map<uint32_t, size_t>& entityComponents = entity->components;
        auto search = entityComponents.find(Component::ID);
        if (search != entityComponents.end()) {
            return (Component*) &components[Component::ID][search.second];
        }

        return nullptr;
    }

    template<typename Component>
    const std::vector<Component*>& query(std::set<uint32_t> req) {
        static_assert(std::is_base_of<ECSComponent<Component>, Component>::value, "Provided template argument must be derived from ECSComponent");
        std::vector<Component*> res;

        if (req.size() == 0) {
            return res;
        }

        std::vector<uint8_t>& componentArray = components[Component::ID];
        if (req.size() == 1) {
            for (size_t i = 0; i < componentArray.size(); i += Component::SIZE) {
                Component* comp = (Component*) &componentArray[i];
                res.push_back(comp);
            }

            return res;
        }

        for (size_t i = 0; i < componentArray.size(); i += Component::SIZE) {
            Component* comp = (Component*) &componentArray[i];

            bool valid = true;
            for (uint32_t id: req) {
                try {
                    comp->entity.at(Component::ID);
                } catch (...) {
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

private:
    void deleteComponent(uint32_t compoonentType, size_t index);

    std::unordered_map<uint32_t, std::vector<uint8_t>> components;
    std::vector<Entity*> entities;
};