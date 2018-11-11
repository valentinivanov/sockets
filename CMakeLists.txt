cmake_minimum_required(VERSION 3.0.0)
project (attoclient)

set(UDP_PRODUCER udp_producer.cpp)
set(UDP_CONSUMER udp_consumer.cpp)
set(TCP_CONSUMER tcp_consumer.cpp)

add_executable(udp_producer ${UDP_PRODUCER})
add_executable(udp_consumer ${UDP_CONSUMER})
add_executable(tcp_consumer ${TCP_CONSUMER})

target_link_libraries(udp_consumer pthread)