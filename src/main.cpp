#include <iostream>
#include <unordered_map>
#include <string>
#include "ECSAdmin.hpp"

struct Name: public ECSComponent<Name> {
    std::string name;
};

struct Age: public ECSComponent<Age> {
    unsigned int age;
};

struct Identifier: public ECSComponent<Identifier> {
    unsigned int id;
};

class PersonSystem: public BaseSystem {
public:
    PersonSystem() {
        setRequirements({Identifier::ID, Name::ID, Age::ID});
    }

    void tick(double timestep) override {
        for (Identifier* id: componentIterator<Identifier>()) {
            Name* name = id->sibling<Name>();
            Age* age = id->sibling<Age>();

            printf("Person %d, age: %d, name: %s\n", id->id, age->age, name->name.c_str());
        }
    }
};

class NamedSystem: public BaseSystem {
public:
    NamedSystem() {
        setRequirements({Identifier::ID, Name::ID});
    }

    void tick(double timestep) override {
        for (Identifier* id: componentIterator<Identifier>()) {
            Name* name = id->sibling<Name>();

            printf("Entity %d, name: %s\n", id->id, name->name.c_str());
        }
    }
};

template <class...T>
void printNumber(T...args) {
    printf("%d\n", sizeof...(args));
}

void testEntities() {
    ECSAdmin admin;

    std::cout << "Identifier: " << Identifier::ID << std::endl;
    std::cout << "Name: " << Name::ID << std::endl;
    std::cout << "Age: " << Age::ID << std::endl << std::endl;

    admin.createEntity(
        Identifier {.id = 0},
        Name {.name="Horacio"},
        Age {.age=18}
    );

    EntityHandle person1 = admin.createEntity();
    admin.addComponent(person1, Identifier{.id = 1});
    admin.addComponent(person1, Name{.name = "Maria"});
    admin.addComponent(person1, Age{.age = 19});

    EntityHandle person2 = admin.createEntity();
    admin.addComponent(person2, Identifier{.id = 2});
    admin.addComponent(person2, Name{.name = "Jose"});
    admin.addComponent(person2, Age{.age = 20});

    EntityHandle named1 = admin.createEntity();
    admin.addComponent(named1, Identifier{.id = 3});
    admin.addComponent(named1, Name{.name = "Name1"});

    EntityHandle named2 = admin.createEntity();
    admin.addComponent(named2, Identifier{.id = 4});
    admin.addComponent(named2, Name{.name = "Name2"});

    PersonSystem personSystem;
    NamedSystem namedSys;
    admin.tickSystems(0, {&personSystem, &namedSys});

    puts("");
    admin.removeComponent<Age>(person2);
    admin.tickSystems(0, {&personSystem, &namedSys});
}

int main() {
    testEntities();

    return 0;
}
