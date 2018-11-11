#include <stdio.h>
#include <stdlib.h>

#include "data_structs.hpp"

#include "service_discovery.h"
#include "udp_multicast_sender.hpp"
#include "container_impl.h"
#include "message_constants.h"

void next_message(vi::Message& m)
{
    m.MessageData = rand() % 100 + 1;
    m.MessageId = m.MessageId + 1;
    m.MessageSize = sizeof(m);
    m.MessageType = PAYLOAD_MESSAGE_TYPE;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    printf("UDP Multicast Producer started...\n");
    printf("Binding to interface: %s, group: %s, port: %d...\n", VI_INTERFACE_ADDRESS, VI_MULTICAST_GROUP, VI_MULTICAST_PORT);

    vi::udp_multicast_sender s;
    s.connection().open()
                  .reuse_address()
                  .set_nonblocking()
                  .disable_loopback()
                  .bind_interface(VI_INTERFACE_ADDRESS);
    s.set_target(VI_MULTICAST_GROUP, VI_MULTICAST_PORT);

    printf("Begin sending packets...\n");

    vi::Message m;
    next_message(m);
    while(m.MessageId < HIMPL_MAX_ELEMENT_COUNT + 1)
    {
        if (vi::udp_multicast_sender::send_ok == s.send<vi::Message>(m))
        {
            next_message(m);
        }
    }

    // send 10 termination messages, just to make sure they've reached recipient
    m.MessageType = TERMINATION_MESSAGE_TYPE;
    for (int i = 0; i < 10; ++i)
    {
        m.MessageData = TARGET_DATA_VALUE;
        s.send<vi::Message>(m);
    }
    
    printf("UDP Multicast Producer finished...\n");
}