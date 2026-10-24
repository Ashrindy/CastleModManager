#include "app.h"
#include "file-dialog.h"
#include "frame-limiter.h"
#include "ui/main-ui.h"

#include <INIReader.h>

#include <algorithm>
#include <fstream>
#include <random>

#ifdef __WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

Application* Application::instance = nullptr;

static double maxFPS{ 60 };
static double maxDeltaTime{ (1.0 / maxFPS) * 1000 };

void Application::init() {
    settings = new Settings{};
    settings->init();
    settings->deserialize();
    setLanguage(settings->languageName.c_str());

	graphics = new Graphics{};
	graphics->init();
    graphics->renderFunc = renderUI;

    modLoaderHandler = new ModLoaderHandler{};
    modLoaderHandler->init();

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    maxFPS = mode->refreshRate;
    maxDeltaTime = (1.0 / maxFPS) * 1000;

    modsFolder = settings->modsFolderPath;
    loadMods(modsFolder);

    instance = this;
}

using sc = std::chrono::system_clock;

void Application::loop() {
    FrameLimiter frameLimiter{ maxDeltaTime };
    while (!glfwWindowShouldClose(graphics->window)) {
        frameLimiter.update();
        update();
        graphics->render();
    }
}

void Application::loadMods(const std::filesystem::path& modsFolder) {
    std::filesystem::path mainIniPath{ modsFolder / "mods.ini" };
    if (std::filesystem::exists(mainIniPath)) {
        INIReader mainIni{ mainIniPath.string() };

        std::vector<std::string> activeModIds{};
        int activeModCount = mainIni.GetInteger("Main", "ActiveModCount", 0);
        for (int i = 0; i < activeModCount; i++) {
            char key[32];
            snprintf(key, sizeof(key), "ActiveMod%d", i);
            activeModIds.emplace_back(mainIni.GetString("Main", key, ""));
        }

        std::vector<std::string> modKeys = mainIni.Keys("Mods");

        for (auto& key : modKeys) {
            Mod& mod = mods.emplace_back();
            mod.id = key;

            auto it = std::find_if(activeModIds.begin(), activeModIds.end(),
                [key](const std::string& m) {
                    return m == key;
                });
            mod.isEnabled = it != activeModIds.end();
            
            std::string modIniPath = mainIni.GetString("Mods", key, "");
            if (std::filesystem::exists(modIniPath)) {
                INIReader modIni{ modIniPath };
                mod.name = modIni.GetString("Desc", "Name", "");
                mod.version = modIni.GetString("Desc", "Version", "");
                mod.author = modIni.GetString("Desc", "Author", "");
                mod.folderPath = std::filesystem::path{ modIniPath }.parent_path();
            }
        }

        auto modsFolderItBegin = std::filesystem::directory_iterator{ modsFolder };
        auto modsFolderItEnd = std::filesystem::directory_iterator{};
        std::vector<std::filesystem::directory_entry> missingMods{};
        std::copy_if(modsFolderItBegin, modsFolderItEnd, std::back_inserter(missingMods), [](const std::filesystem::directory_entry& entry) {
            return std::filesystem::exists(entry.path() / "mod.ini");
        });

        if (missingMods.size() != modKeys.size()) {
            for (auto& x : missingMods) {
                bool exists{ false };
                for (auto& key : modKeys) {
                    if ((x.path() / "mod.ini") == mainIni.GetString("Mods", key, "")) {
                        exists = true;
                        break;
                    }
                }
                if (exists)
                    continue;

                Mod& mod = mods.emplace_back();
                std::random_device rd;
                std::vector<char> id{ 8 };
                std::generate(id.begin(), id.end(), std::ref(rd));
                mod.id = std::to_string(*(int64_t*)id.data());

                mod.isEnabled = false;
                    
                std::string modIniPath = (x.path() / "mod.ini").string();
                if (std::filesystem::exists(modIniPath)) {
                    INIReader modIni{ modIniPath };
                    mod.name = modIni.GetString("Desc", "Name", "");
                    mod.version = modIni.GetString("Desc", "Version", "");
                    mod.author = modIni.GetString("Desc", "Author", "");
                    mod.folderPath = std::filesystem::path{ modIniPath }.parent_path();
                }
            }

            saveMods(modsFolder);
        }
    }
}

void Application::saveMods(const std::filesystem::path& modsFolder) {
    std::filesystem::path mainIniPath{ modsFolder / "mods.ini" };
    std::ofstream mainIni{ mainIniPath };

    mainIni << "[Main]\n";
    mainIni << "Version=1.0\n";
    int activeModCount = 0;
    for (int i = 0; i < (int)mods.size(); i++) {
        auto& mod = mods[i];
        if (mod.isEnabled) {
            char key[32];
            snprintf(key, sizeof(key), "ActiveMod%d", activeModCount);
            mainIni << key << "=" << mod.id << '\n';
            activeModCount++;
        }
    }
    mainIni << "ActiveModCount=" << std::to_string(activeModCount) << '\n';
    mainIni << '\n';

    mainIni << "[Mods]\n";
    for (auto& mod : mods)
        mainIni << mod.id << "=" << (mod.folderPath / "mod.ini").string() << "\n";
}

void Application::deleteMod(int index) {
    std::filesystem::remove_all(mods[index].folderPath);
    mods.erase(mods.begin() + index);
}

void Application::revealMod(int index) {
    auto& path = mods[index].folderPath;

#ifdef __WIN32
    ShellExecuteA(nullptr, "open", path.string().c_str(), nullptr, nullptr, SW_SHOW);
#else
    std::string cmd{ "xdg-open \"" + path.string() + "\"" };
    std::system(cmd.c_str());
#endif
}

void Application::revealMod(const std::string& name) {
    auto it = std::find_if(mods.begin(), mods.end(), [name](const Mod& mod){
        return mod.name == name;
    });
    revealMod(std::distance(mods.begin(), it));
}

void Application::createMod() {
    auto newModDir = std::filesystem::path{ settings->modsFolderPath } / "new-mod";
    std::filesystem::create_directory(newModDir);
    
    auto newModIni = newModDir / "mod.ini";
    std::ofstream newIni{ newModIni };

    newIni << "[Desc]\n";
    newIni << "Name=New Mod\n";
    newIni << "Author=";
#ifdef __WIN32
#else
    newIni << getlogin() << "\n";
#endif
    newIni << "Version=1.0\n\n";
    newIni << "[Main]\n";
    newIni << "DLLFile=\n";
}

void Application::launchGame(const std::filesystem::path& gamePath) {
#ifdef __WIN32
    STARTUPINFO si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);

    CreateProcess(
        gamePath.string().c_str(),
        nullptr, 
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        gamePath.parent_path().string().c_str(),
        &si,
        &pi
    );

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#else
    pid_t pid = fork();

    if (pid == 0) {
        chdir(gamePath.parent_path().string().c_str());
        if (setenv("WINEDLLOVERRIDES", "d3d8=n,b", 1) == -1) {
            perror("setenv");
            _exit(127);
        }

        auto gamePathS = gamePath.string();
        char* args[] = {
#ifdef ENABLE_FLATPAK
            (char*)"flatpak-spawn",
            (char*)"--host",
#endif
            (char*)"wine",
            (char*)gamePathS.c_str(),
            nullptr
        };

#ifdef ENABLE_FLATPAK
        execvp("flatpak-spawn", args);
#else
        execvp("wine", args);
#endif
        _exit(127);
    }
#endif
}

Application::~Application() {
    delete graphics;
    delete modLoaderHandler;
    delete settings;
}

void Application::update() {
}
