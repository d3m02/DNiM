add_library(DnIModules STATIC)
add_library(DnIM ALIAS DnIModules)

target_sources(DnIModules
    PUBLIC FILE_SET HEADERS
    BASE_DIRS 
        ${CMAKE_CURRENT_BINARY_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}
    FILES 
        CrashHandler.hpp
        Logger.hpp
    PRIVATE
        CrashHandler.cpp
        Logger.cpp
)