#pragma once
#include <vector>
#include "settings.h"
#include "ui/graphics/graphics.h"
#include "modloader-handler.h"

struct Mod {
	std::string name;
	std::string version;
	std::string author;
	std::string id;
	std::filesystem::path folderPath;
	bool isEnabled;
};

class Application {
public:
	static Application* instance;

	Graphics* graphics;
	Settings* settings;
	ModLoaderHandler* modLoaderHandler;
	std::filesystem::path modsFolder{};
	std::vector<Mod> mods;

	void init();
	void loop();
	void loadMods(const std::filesystem::path& modsFolder);
	void saveMods(const std::filesystem::path& modsFolder);
	inline void reloadMods(const std::filesystem::path& modsFolder) {
		mods.clear();
        loadMods(modsFolder);
	}
	void launchGame(const std::filesystem::path& gamePath);
	~Application();

private:
	void update();
};
