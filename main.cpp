// WitcherTracker.hpp
#ifndef WITCHER_TRACKER_HPP
#define WITCHER_TRACKER_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <sstream>

namespace Utils {
    // Trim leading and trailing whitespace
    inline void trim(std::string& s) {
        auto notSpace = [](int ch) { return !std::isspace(ch); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
        s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    }

    // Case-insensitive search and comparison
    inline bool containsCI(const std::string& haystack, const std::string& needle) {
        return std::search(haystack.begin(), haystack.end(),
            needle.begin(), needle.end(),
            [](char a, char b){ return std::tolower(a) == std::tolower(b); })
            != haystack.end();
    }
    inline bool equalsCI(const std::string& a, const std::string& b) {
        return a.size() == b.size() &&
            std::equal(a.begin(), a.end(), b.begin(),
                [](char x, char y){ return std::tolower(x)==std::tolower(y); });
    }
}

// Generic item
class Item {
public:
    Item(std::string name_ = "", int quantity_ = 0)
        : name(std::move(name_)), quantity(quantity_) {}
    const std::string& getName() const { return name; }
    int getQuantity() const { return quantity; }
    void setQuantity(int q) { quantity = q; }
private:
    std::string name;
    int quantity;
};

// Inventory management
class Inventory {
public:
    void addItem(const std::string& name, int qty);
    bool removeItem(const std::string& name, int qty) const;
    bool hasEnough(const std::string& name, int qty) const;
    void sortByNameCI();
    const std::vector<Item>& getItems() const { return items; }
private:
    std::vector<Item> items;
};

// Potion formula
class Formula {
public:
    Formula(std::string name_ = "", std::vector<Item> comps_ = {})
        : potionName(std::move(name_)), components(std::move(comps_)) {}
    const std::string& getPotionName() const { return potionName; }
    const std::vector<Item>& getComponents() const { return components; }
    void sortComponents();
private:
    std::string potionName;
    std::vector<Item> components;
};

// Collection of formulas
class FormulaBook {
public:
    void addFormula(const Formula& f);
    Formula* findFormula(const std::string& name);
    const std::vector<Formula>& getAll() const { return formulas; }
private:
    std::vector<Formula> formulas;
};

// Bestiary entry
class BestiaryEntry {
public:
    BestiaryEntry(std::string m = "", std::string p = "", std::string s = "")
        : monster(std::move(m)), potion(p), sign(s) {}
    const std::string& getMonster() const { return monster; }
    const std::string& getPotion() const { return potion; }
    const std::string& getSign() const { return sign; }
    void setPotion(const std::string& p) { potion = p; }
    void setSign(const std::string& s) { sign = s; }
private:
    std::string monster, potion, sign;
};

// Bestiary management
class Bestiary {
public:
    void addEntry(const BestiaryEntry& e);
    BestiaryEntry* findEntry(const std::string& monster);
    const std::vector<BestiaryEntry>& getAll() const { return entries; }
private:
    std::vector<BestiaryEntry> entries;
};

// Item classification
class Classifier {
public:
    static bool isTrophy(const Item& item) {
        const auto& n = item.getName();
        return n.size() >= 7 && Utils::equalsCI(n.substr(n.size()-7), " trophy");
    }
    static bool isPotion(const Item& item, const FormulaBook& book) {
        if (isTrophy(item)) return false;
        for (auto& f : book.getAll())
            if (Utils::equalsCI(f.getPotionName(), item.getName())) return true;
        return false;
    }
};

// Main engine
class WitcherTracker {
public:
    void addToInventory(const std::string& n, int q) { inv.addItem(n,q); }
    void addFormula(const Formula& f) { fb.addFormula(f); }
    void addBestiary(const BestiaryEntry& e) { be.addEntry(e); }

    bool processQuery(const std::string& input);
    bool processAction(const std::string& input) {
        if(dispatchLoot(input)) return true;
        if(dispatchTrade(input)) return true;
        if(dispatchBrew(input)) return true;
        if(dispatchLearn(input)) return true;
        if(dispatchEncounter(input)) return true;
        return false;
    }

private:
    Inventory inv;
    FormulaBook fb;
    Bestiary be;

    bool dispatchLoot(const std::string&);
    bool dispatchTrade(const std::string&);
    bool dispatchBrew(const std::string&);
    bool dispatchLearn(const std::string&);
    bool dispatchEncounter(const std::string&);

    bool dispatchEffectiveness(const std::string&);
    bool dispatchTotalInventory(const std::string&, bool);
    bool dispatchList(const std::string&, const std::string&);
    bool dispatchFormula(const std::string&);
};

// inline implementations for Learn
inline bool WitcherTracker::dispatchLearn(const std::string& input) {
    const std::string pre = "Geralt learns ";
    if(input.rfind(pre,0)!=0) return false;
    std::string part = input.substr(pre.size()); Utils::trim(part);
    // Effectiveness learning
    auto posE = part.find(" is effective against ");
    if(posE!=std::string::npos) {
        std::string counter = part.substr(0,posE);
        std::string rest = part.substr(posE+21);
        Utils::trim(counter); Utils::trim(rest);
        std::istringstream iss(counter);
        std::string cnt,type; iss>>cnt>>type;
        bool asSign = Utils::equalsCI(type,"sign");
        bool asPotion = Utils::equalsCI(type,"potion");
        auto* ent = be.findEntry(rest);
        if(!ent) {
            BestiaryEntry ne(rest, asPotion?cnt:"", asSign?cnt:"");
            be.addEntry(ne);
            std::cout<<"New bestiary entry added: "<<rest<<"\n";
        } else {
            if(asSign) {
                if(ent->getSign().empty()||!Utils::equalsCI(ent->getSign(),cnt)) {
                    ent->setSign(cnt);
                    std::cout<<"Bestiary entry updated: "<<rest<<"\n";
                } else std::cout<<"Already known effectiveness\n";
            } else if(asPotion) {
                if(ent->getPotion().empty()||!Utils::equalsCI(ent->getPotion(),cnt)) {
                    ent->setPotion(cnt);
                    std::cout<<"Bestiary entry updated: "<<rest<<"\n";
                } else std::cout<<"Already known effectiveness\n";
            }
        }
        return true;
    }
    // Formula learning
    auto posF = part.find(" consists of ");
    if(posF!=std::string::npos) {
        std::string pname = part.substr(0,posF);
        std::string comps = part.substr(posF+12);
        Utils::trim(pname); Utils::trim(comps);
        std::vector<Item> items;
        std::istringstream ss(comps); std::string tok;
        while(std::getline(ss,tok,',')){
            Utils::trim(tok);
            std::istringstream is(tok);
            int q; std::string nm;
            is>>q>>std::ws; std::getline(is,nm);
            items.emplace_back(nm,q);
        }
        if(fb.findFormula(pname)) {
            std::cout<<"Already known formula\n";
        } else {
            fb.addFormula(Formula(pname,items));
            std::cout<<"New alchemy formula obtained: "<<pname<<"\n";
        }
        return true;
    }
    return false;
}

// inline implementation for Encounter
inline bool WitcherTracker::dispatchEncounter(const std::string& input) {
    const std::string pre = "Geralt encounters a ";
    if(input.rfind(pre,0)!=0) return false;
    std::string monster = input.substr(pre.size()); Utils::trim(monster);
    auto* ent = be.findEntry(monster);
    if(!ent) {
        std::cout<<"Geralt is unprepared and barely escapes with his life\n";
        return true;
    }
    bool hasEffective = !ent->getSign().empty();
    if(!ent->getPotion().empty() && inv.hasEnough(ent->getPotion(),1)) hasEffective = true;
    if(!hasEffective) {
        std::cout<<"Geralt is unprepared and barely escapes with his life\n";
        return true;
    }
    if(!ent->getPotion().empty() && inv.hasEnough(ent->getPotion(),1)) {
        inv.removeItem(ent->getPotion(),1);
    }
    std::string trophy = monster + " trophy";
    inv.addItem(trophy,1);
    std::cout<<"Geralt defeats "<<monster<<"\n";
    return true;
}

// Main entry point implementing the input loop
inline void runWitcherTracker() {
    WitcherTracker tracker;
    std::string line;
    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        // Dispatch query or action
        if (!line.empty() && line.back() == '?') {
            if (!tracker.processQuery(line)) {
                std::cout << "INVALID
";
            }
        } else {
            if (!tracker.processAction(line)) {
                // Check for Exit
                if (std::equal(line.begin(), line.end(), "Exit", [](char a, char b){ return std::tolower(a)==std::tolower(b); })) {
                    break;
                }
                std::cout << "INVALID
";
            }
        }
    }
}

int main() {
    runWitcherTracker();
    return 0;
}

#endif // WITCHER_TRACKER_HPP
