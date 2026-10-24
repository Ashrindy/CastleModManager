#include "graphics.h"
#include "themes.h"
#include <vector>
#include "../../res/Inter.h"
#include "../../res/icon.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Graphics* Graphics::instance = nullptr;

#ifdef __WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#endif

void Graphics::setOSDecoration() {
#ifdef __WIN32
    HWND hwnd = glfwGetWin32Window(window);
    BOOL darkMode = true;
    DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));

    if (!IsWindowVisible(hwnd) || IsIconic(hwnd))
        return;

    RECT r{};
    if (!GetWindowRect(hwnd, &r))
        return;

    if (IsZoomed(hwnd))
        return;

    int width = r.right - r.left;
    int height = r.bottom - r.top;

    SetWindowPos(
        hwnd, nullptr,
        0, 0, width - 1, height,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
    );
#endif
}

void Graphics::getDpiScale() {
#ifdef __WIN32
	HWND hwnd = glfwGetWin32Window(window);
	if (hwnd) {
		auto dpi = GetDpiForWindow(hwnd);
		dpiScale = dpi / 96.0f;
	}
#else
	// TODO: Figure this out on Linux.
#endif
}

void Graphics::setWindowIcon() {
    int width, height, channels;
    unsigned char* pixels = stbi_load_from_memory(
        (unsigned char*)castlemm_icon_256x256, 
        sizeof(castlemm_icon_256x256), 
        &width, 
        &height, 
        &channels, 
        4
    );
    
    if (!pixels)
        return;

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(window, 1, images);

    stbi_image_free(pixels);
}

bool Graphics::init() {
    if (!glfwInit())
        return false;

    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    if (!nativeWindowBar) {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    } else {
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    }
    window = glfwCreateWindow(720, 680, "Castle Mod Manager", NULL, NULL);
    if (!window) return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    setOSDecoration();
    getDpiScale();
    setWindowIcon();
#ifdef __WIN32
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        Graphics::instance->render();
    });
#endif
    //glfwSetWindowPos(window, 150, 150);
    glfwSetWindowSizeLimits(window, 470, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF((void*)Inter_compressed_data, Inter_compressed_size, 16 * dpiScale);
    largerFont = io.Fonts->AddFontFromMemoryCompressedTTF((void*)Inter_compressed_data, Inter_compressed_size, 38 * dpiScale);
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
#ifdef ENABLE_FLATPAK
    io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
#endif

    ImGui::StyleColorsDark();
    setTheme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    instance = this;

    return true;
}

void Graphics::renderBegin() {
begin:
    glfwPollEvents();

    glfwGetFramebufferSize(window, &width, &height);
    if (width == 0 || height == 0) {
        glfwWaitEvents();
        goto begin;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (renderFunc)
        renderFunc();
}

void Graphics::renderEnd() {
    ImGui::Render();
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    ImVec4 clearColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

Graphics::~Graphics() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
