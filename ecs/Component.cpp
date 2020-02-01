#include "Component.hpp"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>& BaseECSComponent::getComponentTypes() {
    static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>> componentTypes;

    return componentTypes;
}

uint32_t BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn, 
        ECSComponentFreeFunction freefn, size_t SIZE) {
    
    uint32_t componentID = getComponentTypes().size();
    getComponentTypes().push_back(std::make_tuple(createfn, freefn, SIZE));

    return componentID;
}