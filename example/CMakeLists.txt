add_executable(test-main main.cpp)
target_link_libraries(test-main mvvm)
add_custom_command(
    TARGET test-main
    POST_BUILD
    COMMAND $<TARGET_FILE:test-main>
)
