add_executable(${PROJECT_NAME}
	main.c
)
target_link_libraries(${PROJECT_NAME} io libhell-static)
try_strip(${PROJECT_NAME})
install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
)