source_group(tcp_connection_demo REGULAR_EXPRESSION "*")

add_executable (tcp_client tcp_client_demo.cpp)
target_link_libraries (tcp_client network_lib)

add_executable (tcp_server tcp_server_demo.cpp)
target_link_libraries (tcp_server network_lib)