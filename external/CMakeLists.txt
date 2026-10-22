include(FetchContent)

set(GLFW_BUILD_DOCS OFF)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

add_subdirectory(imgui)
add_subdirectory(nativefiledialog-extended)
add_subdirectory(inih)
add_subdirectory(stb_image)
