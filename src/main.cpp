#include <iostream>
#include <unordered_map>
#include "Component.hpp"

struct Comp1 : public ECSComponent<Comp1> {
    int x = 0;
    int y = 0;
};

int main() {
    

    return 0;
}