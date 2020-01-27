#include "Component.hpp"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>> BaseECSComponent::componentTypes;

uint32_t BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn, 
        ECSComponentFreeFunction freefn, size_t SIZE) {
    
    uint32_t componentID = componentTypes.size();
    componentTypes.push_back(std::make_tuple(createfn, freefn, SIZE));

    return componentID;
}