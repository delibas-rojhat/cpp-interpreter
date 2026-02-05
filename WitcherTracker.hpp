#pragma once
#include "Item.hpp"
#include "Potion.hpp"
#include "BestiaryEntry.hpp"
#include <vector>

class WitcherTracker {
    //Dynamic arrays
    std::vector<Item> inventory;                //to store items and trophies
    std::vector<Potion> potionBook;             //to store potions
    std::vector<BestiaryEntry> bestiary;        //to store bestiaries

    //Utility functions
    void trim(std::string& str);
    bool endsWithQuestionMark(const std::string& str);
    const char* strcasestr_custom(const std::string& haystack, const std::string& needle);
    bool compareItems(const Item& a, const Item& b);
    bool caseInsensitiveLess(const std::string& a, const std::string& b);
    bool compareTrophies(const Item& a, const Item& b);
    bool compareComponents(const Item& a, const Item& b);
    bool caseInsensitiveEquals(const std::string& a, const std::string& b);
    void addItem(const std::string& name, int quantity);
    bool removeItem(const std::string& name, int quantity);
    bool hasEnoughItem(const std::string& name, int quantity);
    bool endsWithCaseInsensitive(const std::string& str, const std::string& suffix);
    bool isPotion(const Item& item);
    bool isTrophy(const Item& item);
    bool processLoot(std::string input);



    // public:


    //     // Game actions
    //     void processLoot(const std::vector<Item>& items);
    //     void processTrade(const std::vector<Item>& trophies, const std::vector<Item>& ingredients);
    //     void processBrew(const std::string& potionName);
    //     void processLearn(const std::string& knowledge);
    //     void processEncounter(const std::string& monster);
        
    //     // Queries
    //     void processQuery(const std::string& input);
};