#include "settings.h"

#include <algorithm>
#include <unordered_map>
#include <cstring>

#include "res/lang/en-US.h"

typedef std::pair<const char*, const char*>* LanguageSet;
const char* currentLang{ "en-US" };
LanguageSet currentLangSet{ en_US };
constexpr int langSetSize{ IM_ARRAYSIZE(en_US) };
std::pair<const char*, LanguageSet> langSets[] {
    { "en-US", en_US }
};
constexpr int langSetCount{ IM_ARRAYSIZE(langSets) };
std::unordered_map<std::string_view, const char*> langHashMap;

const char* getLanguageName(const char* langId) {
    const auto it = langNameHashMap.find(std::string_view{ langId });

    if (it != langNameHashMap.end())
        return it->second;
    
    return "MISSING";
}

void setLanguage(const char* lang) {
    currentLang = lang;
    for (auto x = 0; x < langSetCount; x++) {
        auto& langSet = langSets[x];
        if (strcmp(langSet.first, lang) == 0) {
            currentLangSet = langSet.second;

            langHashMap.clear();
            langHashMap.reserve(langSetSize);

            for (auto j = 0; j < langSetSize; ++j) {
                langHashMap.emplace(
                    currentLangSet[j].first,
                    currentLangSet[j].second
                );
            }
            break;
        }
    }
}

const char* translate(const char* id) {
    const auto it = langHashMap.find(std::string_view{ id });

    if (it != langHashMap.end())
        return it->second;
    
    return "MISSING";
}
