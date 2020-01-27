#pragma once

#include <unordered_map>
#include "ComponentTypes.hpp"

typedef std::unordered_map<uint32_t, std::pair<size_t, BaseECSComponent*>> EntityComponentsMap;

struct Entity {
    size_t index;
    EntityComponentsMap components;
};