cmake_minimum_required(VERSION 3.12)
project(BPX 
        VERSION 1.0.0
        DESCRIPTION "BPX Image Processing Library"
        LANGUAGES CXX)

# Configuration options
option(BPX_INSTALL "Install BPX library" OFF)
option(BPX_BUILD_EXAMPLES "Build BPX examples" ${PROJECT_IS_TOP_LEVEL})

# Library target
add_library(${PROJECT_NAME} STATIC
    src/generation.cpp
    src/algorithm.cpp
    src/image.cpp
)

# Modern CMake target properties
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Set C++ standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

# Create an alias target for better usage in other projects
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Examples
if(BPX_BUILD_EXAMPLES)
    find_package(SDL2 QUIET)
    if(NOT SDL2_FOUND)
        message(WARNING "SDL2 not found, examples will not be built. The BPX library will still be built.")
    else()
        add_executable(sdl_example examples/sdl_example.cpp)
        target_link_libraries(sdl_example 
            PRIVATE 
                ${PROJECT_NAME}
                SDL2::SDL2
        )
    endif()
endif()

# Installation
if(BPX_INSTALL)
    include(GNUInstallDirs)
    
    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    
    # Generate and install export files
    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    
    # Generate the config file
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    
    # Install the config files
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif()
