#include "modloader-handler.h"
#include "settings.h"
#include <yyjson.h>

ModLoaderHandler* ModLoaderHandler::instance{ nullptr };

void ModLoaderHandler::init() {
    curl = curl_easy_init();
    instance = this;
    refreshDownloadedReleaseId();
}

void ModLoaderHandler::deinit() {
    curl_easy_cleanup(curl);
}

size_t write_data(void* data, size_t size, size_t nmemb, std::vector<char>* buffer) {
    size_t totalSize = size * nmemb;
    size_t oldSize = buffer->size();

    buffer->resize(oldSize + totalSize);
    memcpy(buffer->data() + oldSize, data, totalSize);
    
    return totalSize;
}

inline void setOpt(CURL* curl, const char* url, std::vector<char>& buffer)  {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Castle Mod Manager");
#ifdef __WIN32
    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
#else 
    curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");
#endif
}

void fetchThreadFunc(ModLoaderHandler* modLoader, void (*func)(CURLcode)) {
    auto* curl = modLoader->curl;
    std::vector<char> buffer{};
    setOpt(curl, "https://api.github.com/repos/Ashrindy/CastleML/releases/latest", buffer);
    modLoader->fetching = true;
    CURLcode res = curl_easy_perform(curl);
    modLoader->fetching = false;

    if (res == CURLE_OK) {
        auto* doc = yyjson_read(buffer.data(), buffer.size(), 0);
        auto* root = yyjson_doc_get_root(doc);

        auto* id = yyjson_obj_get(root, "id");
        auto* name = yyjson_obj_get(root, "name");
        auto* changelog = yyjson_obj_get(root, "body");

        auto* assets = yyjson_obj_get(root, "assets");
        auto* asset = yyjson_arr_get_first(assets);
        auto* downloadUrl = yyjson_obj_get(asset, "browser_download_url");

        modLoader->latestReleaseInfo.id = yyjson_get_sint(id);
        strcpy(modLoader->latestReleaseInfo.name, yyjson_get_str(name));
        modLoader->latestReleaseInfo.changelog = yyjson_get_str(changelog);
        modLoader->latestReleaseInfo.downloadUrl = yyjson_get_str(downloadUrl);
    }

    if(func) func(res);
}

void ModLoaderHandler::fetch(void (*func)(CURLcode)) {
    fetchThread = std::thread{ fetchThreadFunc, this, func };
    fetchThread.detach();
}

void downloadThreadFunc(ModLoaderHandler* modLoader, void (*func)(CURLcode, std::vector<char>&)) {
    auto* curl = modLoader->curl;
    std::vector<char> buffer{};
    setOpt(curl, modLoader->latestReleaseInfo.downloadUrl.c_str(), buffer);
    modLoader->downloading = true;
    CURLcode res = curl_easy_perform(curl);
    modLoader->downloading = false;

    if(func) func(res, buffer);
}

void ModLoaderHandler::download(void (*func)(CURLcode, std::vector<char>&)) {
    downloadThread = std::thread{ downloadThreadFunc, this, func };
    downloadThread.detach();
}

void ModLoaderHandler::setDownloadedReleaseId(int64_t id) {
    auto* settings = Settings::instance;
    auto dataPath = std::filesystem::path{ settings->gameFolderPath } / "modloaderver.dat";
    if (std::filesystem::exists(dataPath)) {
        FILE* file = fopen(dataPath.string().c_str(), "rb");
        fwrite(&id, sizeof(int64_t), 1, file);
        fclose(file);
    }
}

void ModLoaderHandler::refreshDownloadedReleaseId() {
    auto* settings = Settings::instance;
    auto dataPath = std::filesystem::path{ settings->gameFolderPath } / "modloaderver.dat";
    if (std::filesystem::exists(dataPath)) {
        FILE* file = fopen(dataPath.string().c_str(), "rb");
        fread(&downloadedReleaseId, sizeof(int64_t), 1, file);
        fclose(file);
    }
}
