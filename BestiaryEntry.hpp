#pragma once
#include <string>

class BestiaryEntry {
    std::string monsterName;
    std::string effectivePotion;
    std::string effectiveSign;
    
    BestiaryEntry(const std::string& monster = "");
};