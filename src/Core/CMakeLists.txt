include(GenerateExportHeader)

add_library(core sample_library.cpp
        FPSCounter.cpp
        CApp.cpp
        rayTracing/qbImage.cpp
        rayTracing/Ray.cpp
        rayTracing/Scene.cpp
        rayTracing/Camera.cpp
        rayTracing/ObjectBase.cpp
        rayTracing/ObjSphere.cpp
        rayTracing/LightBase.cpp
        ../../include/SDLRT/rayTracing/LightBase.hpp
        rayTracing/PointLight.cpp
        ../../include/SDLRT/rayTracing/PointLight.hpp
        rayTracing/GTform.cpp
        ../../include/SDLRT/rayTracing/GTform.hpp
)


add_library(SDLRT::core ALIAS core)

get_target_property(target_name core NAME)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${target_name} PRIVATE -march=native)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target_name} PRIVATE -march=native)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
    target_compile_options(${target_name} PRIVATE /arch:AVX2)
    message(STATUS "Set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
else ()
    message(STATUS "Impossible to set SIMD instructions to AVX2 for target '${target_name}' with '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif ()

target_link_libraries(core PRIVATE
        SDLRT_options
        SDLRT_warnings)
target_link_libraries(core PUBLIC
        fmt::fmt
        spdlog::spdlog
        glm::glm
        SDL3::SDL3
)


target_include_directories(core ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(core PUBLIC cxx_std_20)

set_target_properties(
        core
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(core EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/SDLRT/sample_library_export.hpp)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(core PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif ()
