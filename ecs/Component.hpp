#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <tuple>
#include <unordered_map>
#include "ComponentTypes.hpp"

struct BaseECSComponent;
struct Entity;

typedef std::pair<size_t, BaseECSComponent*> (*ECSComponentCreateFunction)(std::vector<BaseECSComponent*>& pool, Entity* ent, const BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent {
private:
    static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>& getComponentTypes();
public:
    static uint32_t registerComponentType(ECSComponentCreateFunction createfn, 
        ECSComponentFreeFunction freefn, size_t SIZE);

    inline static ECSComponentCreateFunction getTypeCreateFunction(uint32_t id) {
        return std::get<0>(getComponentTypes()[id]);
    }

    inline static ECSComponentFreeFunction getTypeFreeFunction(uint32_t id) {
        return std::get<1>(getComponentTypes()[id]);
    }

    inline static size_t getTypeSize (uint32_t id) {
        return std::get<2>(getComponentTypes()[id]);
    }

    Entity* entity = nullptr;
};

template<typename T>
struct ECSComponent : public BaseECSComponent {
    static const uint32_t ID;
    static const size_t SIZE;
    static const ECSComponentCreateFunction CREATE_FUNC;
    static const ECSComponentFreeFunction FREE_FUNC;
};

#define VALID_COMP(T) static_assert(std::is_base_of<ECSComponent<T>, T>::value, "Provided template argument must be derived from ECSComponent")

template <typename T>
std::pair<size_t, BaseECSComponent*> ECSComponentCREATE(std::vector<BaseECSComponent*>& pool, Entity* ent, const BaseECSComponent* comp) {
    size_t index = pool.size();

    T* component = new T(*(T*)comp);
    component->entity = ent;

    pool.push_back(component);

    return std::make_pair(index, component);
}

template <typename T>
void ECSComponentFREE(BaseECSComponent* comp) {
    T* component = (T*) comp;
    component->~T();
}

template <typename T>
const uint32_t ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCREATE<T>, ECSComponentFREE<T>, sizeof(T)));

template <typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template <typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNC(ECSComponentCREATE<T>);

template <typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNC(ECSComponentFREE<T>);
