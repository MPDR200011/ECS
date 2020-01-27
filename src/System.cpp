#include "System.hpp"

void BaseSystem::setPool(ECSPool* pool) {
    this->pool = pool;
}

void BaseSystem::setRequirements(std::set<uint32_t>& req) {
    requiredComponents = req;
}

void BaseSystem::setRequirements(std::set<uint32_t>&& req) {
    requiredComponents = std::move(req);
}
