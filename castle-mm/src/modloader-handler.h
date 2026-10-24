#pragma once
#include <curl/curl.h>
#include <thread>
#include <vector>
#include <string>

struct ModLoaderReleaseInfo {
    int64_t id{ -1 };
    char name[32];
    std::string changelog;
    std::string downloadUrl;
};

class ModLoaderHandler {
public:
    CURL* curl{ nullptr };
    std::thread fetchThread;
    std::thread downloadThread;
    ModLoaderReleaseInfo latestReleaseInfo{};
    int64_t downloadedReleaseId{ -1 };
    bool fetching{ false };
    bool downloading{ false };

    void init();
    void deinit();
    void fetch(void (*func)(CURLcode));
    void download(void (*func)(CURLcode, std::vector<char>&));
    void uninstall();
    void setDownloadedReleaseId(int64_t downloadVer);
    inline int64_t getDownloadedReleaseId() {
        return downloadedReleaseId;
    }
    void refreshDownloadedReleaseId();

    static ModLoaderHandler* instance;
};
