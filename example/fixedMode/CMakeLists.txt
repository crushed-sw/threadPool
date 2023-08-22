aux_source_directory(. SRC_LIST)

add_executable(fixedMode ${SRC_LIST})

target_link_libraries(fixedMode threadpool)
