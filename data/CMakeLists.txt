set(GRESOURCE_FILE ${GENERATED_DATA_DIR}/${PROJECT_NAME}.gresource.xml)
#set(GSCHEMA_FILE ${GENERATED_DATA_DIR}/${APPLICATION_ID}.gschema.xml)
set(DESKTOP_FILE ${GENERATED_DATA_DIR}/${APPLICATION_ID}.desktop)
set(ICON_FILE ${GENERATED_DATA_DIR}/${PROJECT_NAME}.svg)

configure_file(gresource.xml.in ${GRESOURCE_FILE})
#configure_file(gschema.xml.in ${GSCHEMA_FILE})
configure_file(desktop.in ${DESKTOP_FILE})
configure_file(icons/scalable/icon.svg ${ICON_FILE} COPYONLY)

#compile_schemas(${GSCHEMA_FILE})

install(FILES ${GSCHEMA_FILE} DESTINATION /usr/share/glib-2.0/schemas)
# install(FILES ${DESKTOP_FILE} DESTINATION /usr/share/applications)
# install(FILES ${ICON_FILE} DESTINATION /usr/share/icons/${PROJECT_NAME})