set(REPLACED_MAIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/main.c)
configure_file(main.c.in ${REPLACED_MAIN_FILE})
add_executable(make-meta
	${REPLACED_MAIN_FILE}
)
try_strip(make-meta)
target_link_libraries(make-meta io)