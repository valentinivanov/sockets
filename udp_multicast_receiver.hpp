#ifndef VI_UDP_MULTICAST_RECEIVER
#define VI_UDP_MULTICAST_RECEIVER

#include "common_defs.h"
#include "common_inc.h"
#include "udp_socket.hpp"


namespace vi
{
    class udp_multicast_receiver
    {
    public:

        enum receive_result
        {
            receive_ok,
            receive_failed,
            receive_try_again
        };

    private:

        udp_socket mSocket;

    public:

        udp_multicast_receiver()
        {
        }

        udp_socket& connection()
        {
            return mSocket;
        }

        template<class P>
        udp_multicast_receiver::receive_result receive(P& packet)
        {
            int result = ::read(mSocket.mSocket, &packet, sizeof(packet));

            if (result >= 0)
            {
                return udp_multicast_receiver::receive_ok;
            }
            else
            {
                udp_multicast_receiver::receive_result r = (EWOULDBLOCK == errno) ? udp_multicast_receiver::receive_try_again : udp_multicast_receiver::receive_failed;
                return r;
            }
        }
    };
};

#endif