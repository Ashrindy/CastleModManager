#pragma once
#include <nfd.h>

struct DialogDesc : nfdopendialogu8args_t {
    bool folderDialog{ false };
};

bool openDialog(DialogDesc& dialogDesc, std::filesystem::path& outPath);
