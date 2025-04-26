cmake_minimum_required(VERSION 3.15)
project(MyBook)

enable_language(RC)

set(SOURCES
    main.cpp
    Window.cpp
    Window.h
)

set(RESOURCES
    resources/resources.h
    resources/notepad.rc
    resources/app.ico
)

add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCES})

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        user32.lib
        gdi32.lib
        comdlg32.lib
    )
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE
        _UNICODE
    )
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/ENTRY:wWinMainCRTStartup"
    )
endif()