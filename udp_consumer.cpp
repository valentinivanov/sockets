#include <stdio.h>
#include <stdlib.h>

#include "data_structs.hpp"

#include "service_discovery.h"
#include "udp_multicast_receiver.hpp"
#include "tcp_client.hpp"

#include "container_impl.h"
#include "udp_consumer_task.hpp"
#include "tcp_consumer_forward_task.hpp"
#include "worker_thread.hpp"

int main(int argc, char *argv[])
{
    printf("UDP Multicast Consumer started...\n");

    printf("Initializing containers...\n");

    vi::container_hashset mainStorage;
    vi::container_queue tcpConsumerQueue;

    printf("Binding UDP receiver to interface: %s, group: %s, port: %d...\n", VI_INTERFACE_ADDRESS, VI_MULTICAST_GROUP, VI_MULTICAST_PORT);

    vi::udp_multicast_receiver r;
    r.connection().open()
                  .reuse_address()
                  .set_nonblocking()
                  .listen(VI_MULTICAST_PORT)
                  .join_multicast_group(VI_MULTICAST_GROUP, VI_INTERFACE_ADDRESS);

    printf("Connecting to TCP Consumer on %s:%d\n", VI_INTERFACE_ADDRESS, VI_SERIALIZATION_PORT);

    vi::tcp_client tc;
    tc.connection().open()
                   .reuse_address()
                   .set_nonblocking();

    while(tc.connection().connect(VI_INTERFACE_ADDRESS, VI_SERIALIZATION_PORT) != vi::tcp_socket::ok)
    {
        sleep(0);
    }

    printf("Connection to TCP Consumer established...\n");
                   
    printf("Starting threads...\n");

    vi::udp_consumer_task task0(&mainStorage, &tcpConsumerQueue, &r);
    vi::udp_consumer_task task1(&mainStorage, &tcpConsumerQueue, &r);
    vi::tcp_consumer_forward_task task2(&mainStorage, &tcpConsumerQueue, &tc);

    vi::worker_thread<vi::udp_consumer_task> thread0(task0);
    vi::worker_thread<vi::udp_consumer_task> thread1(task1);
    vi::worker_thread<vi::tcp_consumer_forward_task> thread2(task2);

    thread0.start();
    thread1.start();
    thread2.start();

    thread0.join();
    thread1.join();
    thread2.join();

    printf("UDP Multicast Consumer finished...\n");
}