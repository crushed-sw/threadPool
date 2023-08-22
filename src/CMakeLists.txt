aux_source_directory(. SRC_LIST)

add_library(threadpool ${SRC_LIST})

target_link_libraries(threadpool muduo_net muduo_base)
