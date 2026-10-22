#pragma once
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class Graphics {
public:
	typedef void renderFuncType();

	static Graphics* instance;

	int width, height;
	GLFWwindow* window;
	renderFuncType* renderFunc;
	ImFont* largerFont;
	float dpiScale{ 1 };
	bool nativeWindowBar{ true };

	bool init();
	void renderBegin();
	void renderEnd();
	inline void render() {
		renderBegin();
		renderEnd();
	}
	void getDpiScale();
	void setOSDecoration();
	void setWindowIcon();
	~Graphics();
};
