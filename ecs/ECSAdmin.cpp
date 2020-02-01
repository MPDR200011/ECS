#include "ECSAdmin.hpp"

void ECSAdmin::tickSystems(double timestep, std::vector<BaseSystem*> systems) {
    for (BaseSystem* sys: systems) {
        sys->setPool(this);
        sys->tick(timestep);
    }
}