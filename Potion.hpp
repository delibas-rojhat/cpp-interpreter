#pragma once
#include "Item.hpp"
#include <vector>

class Potion {
    public:
        std::string potionName;
        std::vector<Item> components;
        
        Potion(const std::string& name = "");
};