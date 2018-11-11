#include <stdio.h>
#include <stdlib.h>

#include "tcp_server.hpp"
#include "service_discovery.h"
#include "data_structs.hpp"
#include "message_constants.h"

#define CONNECTION_QUEUE_MAX_SIZE 16

int main(int argc, char *argv[])
{
    vi::tcp_server s;

    printf("TCP Consumer started...\n");
    printf("Binding to interface: %s, port: %d...\n", VI_INTERFACE_ADDRESS, VI_SERIALIZATION_PORT);

    s.connection().open()
                  .reuse_address()
                  .set_nonblocking()
                  .bind_and_listen(VI_INTERFACE_ADDRESS, VI_SERIALIZATION_PORT, CONNECTION_QUEUE_MAX_SIZE);
    
    printf("Waiting client connection...\n");

    while(s.wait_client() != vi::tcp_server::ok)
    {
        sleep(0);
    }

    printf("Client connection established...\n");

    vi::Message m;
    while(true)
    {
        if (s.receive<vi::Message>(m) == vi::tcp_server::ok)
        {
            printf("Message received: id = %llu, data = %llu...\n", m.MessageId, m.MessageData);

            if (TERMINATION_MESSAGE_TYPE == m.MessageType)
            {
                printf("Termination message received, exiting...\n");
                break;
            }
        }
    }

    printf("TCP Consumer finished...\n");
}