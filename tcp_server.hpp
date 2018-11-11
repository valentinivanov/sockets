#ifndef VI_TCP_SERVER
#define VI_TCP_SERVER

#include "tcp_socket.hpp"

namespace vi
{
    class tcp_server
    {
    public:

        enum server_result
        {
            ok,
            try_again,
            error
        };

    private:

        tcp_socket mServerSocket;
        tcp_socket mClientSocket; //only single client supported.

    public:

        tcp_socket& connection()
        {
            return mServerSocket;
        }

        tcp_server::server_result wait_client()
        {
            tcp_socket::socket_result result = mServerSocket.accept(mClientSocket);

            if (tcp_socket::ok == result)
            {
                return tcp_server::ok;
            }
            else if (tcp_socket::try_again == result)
            {
                return tcp_server::try_again;
            }
            else
            {
               return tcp_server::error; 
            }
        }

        template<class T>
        tcp_server::server_result send(const T& packet)
        {
            int result = ::send(mClientSocket.mSocket, &packet, sizeof(packet), 0);

            if (result > 0)
            {
                return tcp_server::ok;
            }
            else
            {
                return (EWOULDBLOCK == errno) ? tcp_server::try_again : tcp_server::error;
            }
        }

        template<class T>
        tcp_server::server_result receive(T& packet)
        {
            int result = ::recv(mClientSocket.mSocket, &packet, sizeof(packet), 0);
            
            if (result > 0)
            {
                return tcp_server::ok;
            }
            else
            {
                return (EWOULDBLOCK == errno) ? tcp_server::try_again : tcp_server::error;
            }
        }
    };
};

#endif