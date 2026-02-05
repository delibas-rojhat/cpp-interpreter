#pragma once
#include <string>

class Item{
    public:
        std::string name;
        int quantity;
        
        Item(const std::string& name = "", int quantity = 0);

        
};
