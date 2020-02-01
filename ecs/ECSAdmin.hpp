#pragma once

#include <vector>
#include "ECSPool.hpp"
#include "System.hpp"

class ECSAdmin: public ECSPool {
public:
    void tickSystems(double timestep, std::vector<BaseSystem*> systems);
};