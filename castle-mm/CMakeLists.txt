project("castle-mm")

if(WIN32)
    set(WINDOWS_RESOURCES "src/res/resource.rc")
endif()

add_executable (castle-mm
  "src/castle-mm.cpp" 
  "src/ui/graphics/graphics.cpp" 
  "src/ui/graphics/graphics.h" 
  "src/ui/graphics/themes.cpp" 
  "src/ui/graphics/themes.h" 
  "src/ui/native-window-bar.inl"
  "src/ui/mods-tab.inl"
  "src/ui/settings-tab.inl"
  "src/ui/main-ui.h"
  "src/ui/main-ui.cpp"
  "src/app.cpp" 
  "src/app.h"
  "src/frame-limiter.cpp" 
  "src/frame-limiter.h" 
  "src/settings.cpp" 
  "src/settings.h" 
  "src/languages.cpp" 
  "src/languages.h" 
  "src/file-dialog.cpp" 
  "src/file-dialog.h" 
  "src/res/Inter.h" 
  "src/res/icon.h" 
  ${WINDOWS_RESOURCES}
)

target_precompile_headers(castle-mm PRIVATE "src/pch.h")
target_compile_features(castle-mm PUBLIC cxx_std_20)
target_include_directories(castle-mm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(castle-mm PRIVATE glfw imgui nfd inih stb_image)
if(LINUX)
  set(OpenGL_GL_PREFERENCE "GLVND")
  find_package(OpenGL REQUIRED)
  target_link_libraries(castle-mm PRIVATE OpenGL::GL)
endif()
if(WIN32)
  target_link_libraries(castle-mm PRIVATE opengl32 dwmapi)
endif()
