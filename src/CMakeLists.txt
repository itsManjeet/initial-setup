add_executable(${PROJECT_NAME}
    main.cxx
        Application.cxx
        Application.hxx
        Window.cxx
        Window.hxx

        ${GENERATED_DATA_DIR}/resources.c
        Page/Page.cxx
        Page/Page.hxx
        Worker.cxx
        Worker.hxx
        Exec/Exec.cxx
        Exec/Exec.hxx
        Page/Welcome/Welcome.cxx
        Page/Welcome/Welcome.hxx
        Page/Method/Method.cxx
        Page/Method/Method.hxx
        Page/Disk/Disk.cxx
        Page/Disk/Disk.hxx
        Page/Disk/DiskColumn.cxx
        Page/Disk/DiskColumn.hxx
        Page/Confirm/Confirm.cxx
        Page/Confirm/Confirm.hxx
        Page/Confirm/ConfigColumn.cxx
        Page/Confirm/ConfigColumn.hxx
        Page/User/User.cxx
        Page/User/User.hxx
        Page/Timezone/Timezone.cxx
        Page/Timezone/Timezone.hxx
        Page/Summary/Summary.cxx
        Page/Summary/Summary.hxx)
target_link_libraries(${PROJECT_NAME}
    PkgConfig::GTKMM)

target_include_directories(${PROJECT_NAME} PRIVATE ${GENERATED_SOURCES_DIR})

compile_resources(
        ${GENERATED_DATA_DIR}/resources.c
        window.glade
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
