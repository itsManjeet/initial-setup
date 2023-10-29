add_executable(${PROJECT_NAME}
    main.cxx
        Application.cxx
        Application.hxx
        Window.cxx
        Window.hxx

        ${GENERATED_DATA_DIR}/resources.c
        Page/Page.cxx
        Page/Page.hxx
        Page/Welcome/Welcome.cxx
        Page/Welcome/Welcome.hxx
        Page/Disk/Disk.cxx
        Page/Disk/Disk.hxx
        Page/Disk/DiskColumn.cxx
        Page/Disk/DiskColumn.hxx
        Exec/Exec.cxx
        Exec/Exec.hxx
        Page/Confirm/Confirm.cxx
        Page/Confirm/Confirm.hxx
        Page/Confirm/ConfigColumn.cxx
        Page/Confirm/ConfigColumn.hxx
        Worker.cxx
        Worker.hxx
        Page/User/User.cxx
        Page/User/User.hxx
        Page/Summary/Summary.cxx
        Page/Summary/Summary.hxx)
target_link_libraries(${PROJECT_NAME}
    GTKMM)

target_include_directories(${PROJECT_NAME} PRIVATE ${GENERATED_SOURCES_DIR})

compile_resources(
        ${GENERATED_DATA_DIR}/resources.c
        window.glade
)