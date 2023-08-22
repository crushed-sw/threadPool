aux_source_directory(. SRC_LIST)

add_executable(cachedMode ${SRC_LIST})

target_link_libraries(cachedMode threadpool)
