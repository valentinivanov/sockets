#ifndef VI_TCP_CLIENT
#define VI_TCP_CLIENT

#include "tcp_socket.hpp"

namespace vi
{
    class tcp_client
    {
    public:

        enum client_result
        {
            ok,
            try_again,
            error
        };

    private:

        tcp_socket mSocket;

    public:

        tcp_socket& connection()
        {
            return mSocket;
        }

        template<class T>
        tcp_client::client_result send(const T& packet)
        {
            int result = ::send(mSocket.mSocket, &packet, sizeof(packet), 0);

            if (result > 0)
            {
                return tcp_client::ok;
            }
            else
            {
                return (EWOULDBLOCK == errno) ? tcp_client::try_again : tcp_client::error;
            }
        }

        template<class T>
        tcp_client::client_result receive(T& packet)
        {
            int result = ::recv(mSocket.mSocket, &packet, sizeof(packet));
            
            if (result > 0)
            {
                return tcp_client::ok;
            }
            else
            {
                return (EWOULDBLOCK == errno) ? tcp_client::try_again : tcp_client::error;
            }
        }
    };
};

#endif