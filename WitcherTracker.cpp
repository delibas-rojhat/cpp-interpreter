#include "Item.hpp"
#include "Potion.hpp"
#include "BestiaryEntry.hpp"
#include "WitcherTracker.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cctype>



//----------------------Utility Functions-------------------------//

//Trims leading and trailing spaces
void WitcherTracker::trim(std::string& str) {
    // Remove leading whitespace
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    // Remove trailing whitespace
    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }

    // Update string to trimmed version
    str = str.substr(start, end - start);
}

//Check if the string (after trimming trailing whitespace) ends with a '?' character.
bool WitcherTracker::endsWithQuestionMark(const std::string& str) {
    int len = static_cast<int>(str.size());

    // Move backwards past trailing whitespace
    while (len > 0 && std::isspace(static_cast<unsigned char>(str[len - 1]))) {
        len--;
    }

    // Check if there's at least one character and if it is '?'
    return (len > 0 && str[len - 1] == '?');
}

// Case-insensitive substring search
const char* WitcherTracker::strcasestr_custom(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return haystack.c_str();

    for (size_t i = 0; i <= haystack.size() - needle.size(); ++i) {
        size_t j = 0;
        while (j < needle.size() && std::tolower(static_cast<unsigned char>(haystack[i + j])) == std::tolower(static_cast<unsigned char>(needle[j]))) {
            ++j;
        }
        if (j == needle.size()) {
            return haystack.c_str() + i;
        }
    }



return nullptr;
}

// Comparator function for sorting
bool WitcherTracker::compareItems(const Item& a, const Item& b) {
    return caseInsensitiveLess(a.name, b.name);
}

// Comparator for sorting trophies
bool WitcherTracker::compareTrophies(const Item& a, const Item& b) {
    std::string monsterA = a.name;
    std::string monsterB = b.name;

    // Remove " trophy" suffix (case-insensitive)
    const std::string suffix = " trophy";

    auto stripSuffix = [](std::string& str, const std::string& suffix) {
        if (str.size() >= suffix.size()) {
            std::string end = str.substr(str.size() - suffix.size());
            std::transform(end.begin(), end.end(), end.begin(), ::tolower);
            if (end == suffix) {
                str.resize(str.size() - suffix.size()); // strip suffix
            }
        }
    };

    stripSuffix(monsterA, suffix);
    stripSuffix(monsterB, suffix);

    return caseInsensitiveLess(monsterA, monsterB);
}

// Comparator for sorting components
bool WitcherTracker::compareComponents(const Item& a, const Item& b) {
    if (a.quantity != b.quantity) {
        return a.quantity > b.quantity; // descending order
    }
    return caseInsensitiveLess(a.name, b.name); // alphabetical if quantities match
}

// Helper function: case-insensitive comparison of two strings
bool WitcherTracker::caseInsensitiveLess(const std::string& a, const std::string& b) {
    size_t len = std::min(a.size(), b.size());
    for (size_t i = 0; i < len; ++i) {
        char lowerA = static_cast<char>(std::tolower(static_cast<unsigned char>(a[i])));
        char lowerB = static_cast<char>(std::tolower(static_cast<unsigned char>(b[i])));
        if (lowerA < lowerB){
            return true;
        }
        if (lowerA > lowerB) {
            return false;
        }
    }
    return a.size() < b.size();
}

// Helper function: case-insensitive checking two string are equal or not
bool WitcherTracker::caseInsensitiveEquals(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Helper function: case-insensitive checking suffix
bool WitcherTracker::endsWithCaseInsensitive(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size()) return false;
    size_t startPos = str.size() - suffix.size();
    for (size_t i = 0; i < suffix.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(str[startPos + i])) !=
            std::tolower(static_cast<unsigned char>(suffix[i]))) {
            return false;
        }
    }
    return true;
}



//------------------Inventory Functions---------------------//

//Adds or updates an item in the inventory.
void WitcherTracker::addItem(const std::string& name, int quantity) {
    for (auto& item : inventory) {
        if (caseInsensitiveEquals(item.name, name)) {
            item.quantity += quantity;
            return;
        }
    }

    // If item not found, just add it
    inventory.push_back({name, quantity});
}

//Removes a given quantity of an item from the inventory.
bool WitcherTracker::removeItem(const std::string& name, int quantity) {
    for (auto& item : inventory) {
        if (caseInsensitiveEquals(item.name, name)) {
            if (item.quantity >= quantity) {
                item.quantity -= quantity;
                return true;
            }
            return false; // Not enough quantity to remove
        }
    }
    return false; // Item not found
}

// Checks if the inventory has at least the required quantity.
bool WitcherTracker::hasEnoughItem(const std::string& name, int quantity) {
    for (const auto& item : inventory) {
        if (caseInsensitiveEquals(item.name, name)) {
            return item.quantity >= quantity;
        }
    }
    return false;
}


//-----------------Classification Helpers----------------------//

//An item is a potion if its name does not end with " trophy" and its name matches one of the known potion formulas.
bool WitcherTracker::isPotion(const Item& item) {
    if (endsWithCaseInsensitive(item.name, " trophy")) {
        return false;
    }

    for (const auto& potion : potionBook) {
        if (caseInsensitiveEquals(item.name, potion.potionName)) {
            return true;
        }
    }
    return false;
}

//An item is a trophy if its name ends with " trophy".
bool WitcherTracker::isTrophy(const Item& item) {
    return endsWithCaseInsensitive(item.name, " trophy");
}

//-----------------Action Handlers-----------------------------//

//Loot Action: "Geralt loots" followed by an ingredient_list.
bool WitcherTracker::processLoot(std::string input) {
    const std::string prefix = "Geralt loots ";
    size_t pos = input.find(prefix);
    if (pos == std::string::npos) return false;  // equivalent to your check for NULL
    
    std::string itemList = input.substr(pos + prefix.length());
    trim(itemList);

    std::stringstream ss(itemList);
    std::string token;

    while (std::getline(ss, token, ',')) {
        trim(token);

        std::stringstream tokenStream(token);
        int quantity;
        std::string name;

        if (!(tokenStream >> quantity) || quantity <= 0) {
            return false; // invalid quantity
        }
        // read the rest of the token as name (handles multi-word names)
        std::getline(tokenStream >> std::ws, name);
        trim(name);

        if (name.empty()) {
            return false; // no name found
        }

        addItem(name, quantity);
    }

    std::cout << "Alchemy ingredients obtained\n";
    return true;
}
