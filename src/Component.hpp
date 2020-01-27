#pragma once

#include <cstdint>
#include <vector>
#include <tuple>

struct BaseECSComponent;
struct Entity;

typedef size_t (*ECSComponentCreateFunction)(std::vector<uint8_t>& memory, Entity* ent, BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent {
public:
    static uint32_t registerComponentType(ECSComponentCreateFunction createfn, 
        ECSComponentFreeFunction freefn, size_t SIZE);

    inline static ECSComponentCreateFunction getTypeCreateFunction(uint32_t id) {
        return std::get<0>(componentTypes[id]);
    }

    inline static ECSComponentFreeFunction getTypeFreeFunction(uint32_t id) {
        return std::get<1>(componentTypes[id]);
    }

    inline static size_t getTypeSize (uint32_t id) {
        return std::get<2>(componentTypes[id]);
    }

    Entity* entity = nullptr;
private:
    static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>> componentTypes;
};

template<typename T>
struct ECSComponent : public BaseECSComponent {
    static const uint32_t ID;
    static const size_t SIZE;
    static const ECSComponentCreateFunction CREATE_FUNC;
    static const ECSComponentFreeFunction FREE_FUNC;

    template <typename Component>
    Component* sibling() {
        //FIXME implement
    }
};

template <typename T>
size_t ECSComponentCREATE(std::vector<uint8_t>& memory, Entity* ent, BaseECSComponent* comp) {
    size_t index = memory.size();
    memory.resize(index + T::SIZE);

    T* component = new(&memory[index])T(*(T*)comp);
    component->entity = ent;

    return index;
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