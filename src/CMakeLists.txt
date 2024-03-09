add_library(network_lib)
set_target_properties(network_lib PROPERTIES LINKER_LANGUAGE CXX)


source_group(includes REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/include/*")
source_group(tcp REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/tcp/*")
source_group(utils REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/utils/*")

target_sources(network_lib PUBLIC
 "${CMAKE_CURRENT_SOURCE_DIR}/include/tcp/tcp.hpp"
)


target_sources(network_lib PRIVATE 
"${CMAKE_CURRENT_SOURCE_DIR}/tcp/socket.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/tcp/socket.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/tcp/tcp_client.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/tcp/tcp_client.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/tcp/tcp_server.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/tcp/tcp_server.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/constants.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/c_wrappers.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/c_wrappers.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/socket_manager.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/utils/socket_manager.cpp"
)

target_include_directories(network_lib PUBLIC   "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                                       PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/tcp" "${CMAKE_CURRENT_SOURCE_DIR}/utils")