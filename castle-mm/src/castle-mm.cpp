#include "app.h"

int main() {
	Application* app = new Application;
	app->init();
	app->loop();
	delete app;
	return 0;
}
