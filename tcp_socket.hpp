#ifndef VI_TCP_SOCKET_HPP
#define VI_TCP_SOCKET_HPP

#include "common_defs.h"
#include "common_inc.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace vi
{
    class tcp_socket
    {
        friend class tcp_client;
        friend class tcp_server;

    public:

        enum socket_result
        {
            ok,
            try_again,
            error
        };

    private:

        socket_t mSocket;
        sockaddr_in mAddress;

    public:

        tcp_socket() : mSocket(-1)
        {
            memset(&mAddress, 0, sizeof(mAddress));
        }

        ~tcp_socket()
        {
            close();
        }

        bool is_valid() const
        {
            return mSocket >= 0;
        }

        tcp_socket& open()
        {
            mSocket = ::socket(AF_INET, SOCK_STREAM, 0);
            assert(mSocket >= 0);

            return *this;
        }

        void close()
        {
            if (is_valid())
            {
                ::close(mSocket);
                mSocket = -1;
            }
        }

        tcp_socket& reuse_address()
        {
            int reuse = 1;

            int result = ::setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
            assert(0 == result);

            return *this;
        }

        tcp_socket& set_nonblocking()
        {
            int flags = ::fcntl(mSocket, F_GETFL, 0);

            if (-1 == flags)
            {
                flags = 0;
            }
        
            int result = ::fcntl(mSocket, F_SETFL, flags | O_NONBLOCK);
            assert(0 == result);

            return *this;
        }

        tcp_socket& bind_and_listen(const char* address, uint16_t port, uint16_t queueSize)
        {
            mAddress.sin_family = AF_INET;
            mAddress.sin_addr.s_addr = ::inet_addr(address);;
            mAddress.sin_port = htons(port);

            int result = ::bind(mSocket, reinterpret_cast<const sockaddr*>(&mAddress), sizeof(mAddress));
            assert(0 == result);

            result = ::listen(mSocket, queueSize);
            assert(0 == result);

            return *this;
        }

        tcp_socket::socket_result connect(const char* address, uint16_t port)
        {
            mAddress.sin_family = AF_INET;
            mAddress.sin_addr.s_addr = ::inet_addr(address);;
            mAddress.sin_port = ::htons(port);

            int result = ::connect(mSocket, reinterpret_cast<const sockaddr*>(&mAddress), sizeof(mAddress));

            return (0 == result) ? tcp_socket::ok : tcp_socket::error;
        }

        tcp_socket::socket_result accept(tcp_socket& s)
        {
            socklen_t addrSize = sizeof(s.mAddress);
            s.mSocket = ::accept(mSocket, reinterpret_cast<sockaddr*>(&mAddress), &addrSize);
            if (s.mSocket < 0)
            {
                return (EWOULDBLOCK == errno) ? tcp_socket::try_again : tcp_socket::error;
            }
            else
            {
                return tcp_socket::ok;
            }
        }
    };
};

#endif