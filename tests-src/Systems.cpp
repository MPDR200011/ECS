#include "Systems.hpp"
#include "Components.hpp"

PersonSystem::PersonSystem() {
    setRequirements({Identifier::ID, Name::ID, Age::ID});
}

void PersonSystem::tick(double timestep)  {
    for (Entity* entity: getEntities()) {
        Identifier* id = getComponent<Identifier>(entity);
        Name* name = getComponent<Name>(entity);
        Age* age = getComponent<Age>(entity);

        printf("Person %d, age: %d, name: %s\n", id->id, age->age, name->name.c_str());
    }
}

NamedSystem::NamedSystem() {
    setRequirements({Identifier::ID, Name::ID});
}

void NamedSystem::tick(double timestep) {
    for (Entity* entity: getEntities()) {
        Identifier* id = getComponent<Identifier>(entity);
        Name* name = getComponent<Name>(entity);

        printf("Entity %d, name: %s\n", id->id, name->name.c_str());
    }
}