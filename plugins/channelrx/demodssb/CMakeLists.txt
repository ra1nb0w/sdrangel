project(ssb)

set(ssb_SOURCES
	ssbdemod.cpp
	ssbdemodsettings.cpp
	ssbplugin.cpp
)

set(ssb_HEADERS
	ssbdemod.h
	ssbdemodsettings.h
	ssbplugin.h
)

include_directories(
        ${CMAKE_SOURCE_DIR}/swagger/sdrangel/code/qt5/client
)

if(NOT SERVER_MODE)
    set(ssb_SOURCES
        ${ssb_SOURCES}
        ssbdemodgui.cpp

        ssbdemodgui.ui
    )
    set(ssb_HEADERS
        ${ssb_HEADERS}
        ssbdemodgui.h
    )

    set(TARGET_NAME demodssb)
    set(TARGET_LIB "Qt5::Widgets")
    set(TARGET_LIB_GUI "sdrgui")
    set(INSTALL_FOLDER ${INSTALL_PLUGINS_DIR})
else()
    set(TARGET_NAME demodssbsrv)
    set(TARGET_LIB "")
    set(TARGET_LIB_GUI "")
    set(INSTALL_FOLDER ${INSTALL_PLUGINSSRV_DIR})
endif()

add_library(${TARGET_NAME} SHARED
	${ssb_SOURCES}
)

target_link_libraries(${TARGET_NAME}
        Qt5::Core
        ${TARGET_LIB}
	sdrbase
	${TARGET_LIB_GUI}
        swagger
)

install(TARGETS ${TARGET_NAME} DESTINATION ${INSTALL_FOLDER})
