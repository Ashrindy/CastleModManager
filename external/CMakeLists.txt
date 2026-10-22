include(FetchContent)

set(GLFW_BUILD_DOCS OFF)
if(ENABLE_FLATPAK)
    find_package(glfw3 CONFIG REQUIRED)

    if(TARGET glfw3::glfw)
        set(CASTLE_GLFW_TARGET glfw3::glfw)
    elseif(TARGET glfw)
        set(CASTLE_GLFW_TARGET glfw)
    else()
        message(FATAL_ERROR "No GLFW target exists")
    endif()
else()
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
  )
  FetchContent_MakeAvailable(glfw)
  set(CASTLE_GLFW_TARGET glfw)
endif()

add_subdirectory(imgui)
add_subdirectory(nativefiledialog-extended)
add_subdirectory(inih)
add_subdirectory(stb_image)
