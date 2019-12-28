add_executable(${PROJECT_NAME}
	main.c
)
target_link_libraries(${PROJECT_NAME} io libhell-static)
if(NOT APPLE)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_STRIP} ${PROJECT_NAME}
	)
endif()
install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
)