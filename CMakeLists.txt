cmake_minimum_required (VERSION 3.18)
project (Scarlatti)

add_subdirectory(external/glfw external/glm-1.0.1 )

set(GLFW_INCLUDE_PATH "external/glfw/include/")

set(GLM_INCLUDE_PATH   "external/glm-1.0.1" )

include_directories(
        ${GLFW_INCLUDE_PATH}
        ${GLM_INCLUDE_PATH}
)

set(MY_HEADER_FILES
        src/core/device.cpp
        src/core/device.h
        src/core/SwapChain.cpp
        src/core/SwapChain.h
        src/core/Window.cpp
        src/core/Window.h
)

set(MY_SOURCE_FILES
        src/main.cpp
        src/core/device.cpp
        src/core/device.h
        src/Common.h
        src/core/SwapChain.cpp
        src/core/SwapChain.h
        src/core/Window.cpp
        src/core/Window.h
)

add_executable(${PROJECT_NAME}
        ${MY_HEADER_FILES}
        ${MY_SOURCE_FILES}
)

find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME}
        glfw
        Vulkan::Vulkan
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}"
)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")


# todo: make the project to work in VS. RN some c++ dependencies are missing when cmake build.

# find the shaders files
file(GLOB_RECURSE shader_files "${CMAKE_SOURCE_DIR}/shaders/*")

# find the compilation shaders file
set(shader_compile_script "${CMAKE_SOURCE_DIR}/CompileShaders.bat")

# copy the compilation file to the binary dir
configure_file(${shader_compile_script} ${CMAKE_BINARY_DIR}/CompileShaders.bat COPYONLY)

# copy the shaders file to the binary dir
foreach(shader ${shader_files})
    get_filename_component(shader_name ${shader} NAME)
    configure_file(${shader} ${CMAKE_BINARY_DIR}/shaders/${shader_name} COPYONLY)
endforeach()

# add a custom target to run the compile shader file
add_custom_target(vk_shaders ALL
        COMMAND cmd.exe /c "${CMAKE_BINARY_DIR}/CompileShaders.bat"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Compilando shaders..."
)


set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DPI_AWARE "PerMonitor")
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDLL)

# test if linux, mac or windows :

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message("You're on Mac OS !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message("You're on Linux !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message("You're on Windows !")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Yes this is weird but you have to repeat the "if" here.


# Add a checkbox in CMakeGUI :

set(SOME_STUFF OFF CACHE BOOL "Check this to print an extra message at the beginning of main()")

# Add a preprocessor define :

if(SOME_STUFF)
    add_definitions(
            -DCOMPILE_WITH_SOME_PREPROCESSOR_DIRECTIVE
    )
endif(SOME_STUFF)