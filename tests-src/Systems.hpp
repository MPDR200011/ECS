#pragma once

#include "System.hpp"

class PersonSystem: public BaseSystem {
public:
    PersonSystem();

    void tick(double timestep) override;
};

class NamedSystem: public BaseSystem {
public:
    NamedSystem();

    void tick(double timestep) override;
};