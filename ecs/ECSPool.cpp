#include "ECSPool.hpp"
#include <cstring>

ECSPool::~ECSPool() {
    for (auto it = components.begin(); it != components.end(); it++) {
        ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(it->first);
        size_t componentSize = BaseECSComponent::getTypeSize(it->first);

        for (size_t i = 0; i < it->second.size(); i++) {
            freefn(it->second[i]);
        }
    }

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
}

EntityHandle ECSPool::createEntity() {
    size_t index = entities.size();

    Entity* entity = new Entity();
    entity->index = index;

    entities.push_back(entity);

    return entity;
}

void ECSPool::removeEntity(EntityHandle entity) {
    for (auto &it: entity->components) {
        deleteComponent(it.first, it.second.first);
    }

    size_t destIndex = entity->index;
    size_t srcIndex = entities.size() - 1;

    Entity* srcEntity = entities[srcIndex];
    Entity* destEntity = entities[destIndex];
    delete destEntity;
    
    entities[destIndex] = srcEntity;
    srcEntity->index = destIndex;

    entities.pop_back();
}

void ECSPool::deleteComponent(uint32_t componentType, size_t index) {
    ComponentPool& pool = components[componentType];

    ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(componentType);

    size_t srcIndex = pool.size() - 1; 
    BaseECSComponent* srcComponent = pool[srcIndex];
    BaseECSComponent* destComponnent = pool[index];
    freefn(destComponnent);

    if (index == srcIndex) {
        pool.pop_back();
        return;
    }

    pool[index] = srcComponent;

    srcComponent->entity->components[componentType].first = index;

    pool.pop_back();
}