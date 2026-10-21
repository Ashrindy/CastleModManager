#pragma once
#include <unordered_map>

static std::unordered_map<std::string_view, const char*> langNameHashMap {
    { "en-US", "English (American)" }
};

const char* getLanguageName(const char* langId);
void setLanguage(const char* lang);
const char* translate(const char* id);
