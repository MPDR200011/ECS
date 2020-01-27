#include "System.hpp"

void BaseSystem::setAdmin(ECS* admin) {
    this->admin = admin;
}

void BaseSystem::setRequirements(std::set<uint32_t>& req) {
    requiredComponents = req;
}

void BaseSystem::setRequirements(std::set<uint32_t>&& req) {
    requiredComponents = std::move(req);
}