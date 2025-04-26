cmake_minimum_required(VERSION 3.15)
project(WinAPIApp LANGUAGES CXX)

# Для Windows приложений
if(WIN32)
    add_executable(${PROJECT_NAME} 
        main.cpp
        Window.cpp
        Window.h
    )
    
    # Критически важные настройки для WinAPI
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/ENTRY:wWinMainCRTStartup"
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        user32.lib
        gdi32.lib
    )
    
    # Для Unicode
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE
        _UNICODE
    )
endif()