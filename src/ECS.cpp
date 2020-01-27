#include "ECS.hpp"
#include <cstring>

ECS::~ECS() {
    for (auto it = components.begin(); it != components.end(); it++) {
        ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(it->first);
        size_t componentSize = BaseECSComponent::getTypeSize(it->first);

        for (size_t i = 0; i < it->second.size(); i += componentSize) {
            freefn((BaseECSComponent*) &it->second[i]);
        }
    }

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
}

EntityHandle ECS::createEntity() {
    size_t index = entities.size();

    Entity* entity = new Entity();
    entity->index = index;

    entities.push_back(entity);

    return entity;
}

void ECS::removeEntity(EntityHandle entity) {
    for (auto &it: entity->components) {
        deleteComponent(it.first, it.second);
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

void ECS::deleteComponent(uint32_t componentType, size_t index) {
    std::vector<uint8_t>& memory = components[index];

    ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(componentType);
    size_t componentSize = BaseECSComponent::getTypeSize(componentType);

    size_t srcIndex = memory.size() - componentSize; 
    BaseECSComponent* srcComponent = (BaseECSComponent*) &memory[srcIndex];
    BaseECSComponent* destComponnent = (BaseECSComponent*) &memory[index];
    freefn(destComponnent);

    if (index == srcIndex) {
        memory.resize(srcIndex);
        return;
    }

    std::memcpy(destComponnent, srcComponent, componentSize);

    srcComponent->entity->components[componentType] = index;

    memory.resize(srcIndex);
}