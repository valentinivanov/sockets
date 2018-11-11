#ifndef VI_UDP_MULTICAST_SENDER
#define VI_UDP_MULTICAST_SENDER

#include "common_defs.h"
#include "common_inc.h"
#include "udp_socket.hpp"


namespace vi
{
    class udp_multicast_sender
    {
    public:

        enum send_result
        {
            send_ok,
            send_failed,
            send_try_again
        };

    private:

        udp_socket mSocket;
        sockaddr_in mTarget;

    public:

        udp_multicast_sender()
        {
            memset(&mTarget, 0, sizeof(mTarget));
        }

        udp_socket& connection()
        {
            return mSocket;
        }

        void set_target(const char* address, uint16_t port)
        {
            mTarget.sin_family = AF_INET;
            mTarget.sin_addr.s_addr = ::inet_addr(address);
            mTarget.sin_port = ::htons(port);
        }

        template<class P>
        udp_multicast_sender::send_result send(const P& packet)
        {
            int result = ::sendto(mSocket.mSocket, &packet, sizeof(packet), 0, (sockaddr*)&mTarget, sizeof(mTarget));

            if (result > 0)
            {
                return udp_multicast_sender::send_ok;
            }
            else
            {
                return (EWOULDBLOCK == errno) ? udp_multicast_sender::send_try_again : udp_multicast_sender::send_failed;
            }
        }
    };
};

#endif