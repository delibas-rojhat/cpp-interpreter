#pragma once
#include <string>
#include <algorithm>

class Utils {
    public:
        std::string& trim(std::string& str);
        bool endsWithQuestionMark(const std::string& str);
        bool strcasestr_custom(const std::string& haystack, const std::string& needle);
        bool compareItems(const Item& a, const Item& b);
        bool caseInsensitiveLess(const std::string& a, const std::string& b);

    std::vector<std::string> split(const std::string& str, char delimiter);
}





