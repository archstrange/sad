aux_source_directory(. SRC)

add_executable(check ${SRC})
target_link_libraries(check sad)

