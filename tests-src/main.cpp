#include <iostream>
#include <unordered_map>
#include <string>
#include "ECSAdmin.hpp"
#include "Components.hpp"
#include "Systems.hpp"

void testEntities() {
    ECSAdmin admin;


    for (int i = 0; i < 200; i++) {
        admin.createEntity(
            Identifier {.id = i},
            Name {.name="Horacio"},
            Age {.age=18}
        );
    }

    PersonSystem personSystem;
    NamedSystem namedSys;
    admin.tickSystems(0, {&personSystem, &namedSys});

    puts("");
    admin.tickSystems(0, {&personSystem, &namedSys});
}

int main() {
    testEntities();

    return 0;
}
