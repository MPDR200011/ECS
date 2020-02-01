#pragma once

#include <string>
#include "Component.hpp"

struct Name: public ECSComponent<Name> {
    std::string name;
};

struct Age: public ECSComponent<Age> {
    unsigned int age;
};

struct Identifier: public ECSComponent<Identifier> {
    unsigned int id;
};