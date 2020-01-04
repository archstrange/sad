aux_source_directory(. SRC)

add_library(sad ${SRC})
target_link_libraries(sad cutils)
