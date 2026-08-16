add_executable(test_lib test_lib.c)
target_link_libraries(test_lib image_processor)
add_test(NAME test_lib COMMAND test_lib)

add_executable(test_app test_app.c)
target_link_libraries(test_app image_processor)
add_test(NAME test_app COMMAND test_app)