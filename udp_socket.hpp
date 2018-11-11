#ifndef VI_UDP_SOCKET_HPP
#define VI_UDP_SOCKET_HPP

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
    class udp_socket
    {
        friend class udp_multicast_receiver;
        friend class udp_multicast_sender;

    private:

        socket_t mSocket;

    public:

        udp_socket() : mSocket(-1)
        {
        }

        ~udp_socket()
        {
            close();
        }

        bool is_valid() const
        {
            return mSocket >= 0;
        }

        udp_socket& open()
        {
            mSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
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

        udp_socket& reuse_address()
        {
            int reuse = 1;

            int result = ::setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
            assert(0 == result);

            return *this;
        }

        udp_socket& set_nonblocking()
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

        udp_socket& listen(uint16_t port)
        {
            sockaddr_in sockAddr = {0};
            
            sockAddr.sin_family = AF_INET;
            sockAddr.sin_port = ::htons(port);
            sockAddr.sin_addr.s_addr  = INADDR_ANY;

            int result = ::bind(mSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
            assert(0 == result);

            return *this;
        }

        udp_socket& join_multicast_group(const char* groupAddress, 
                                         const char* interfaceAddress)
        {
            ip_mreq group;

            group.imr_multiaddr.s_addr = ::inet_addr(groupAddress);
            group.imr_interface.s_addr = ::inet_addr(interfaceAddress);

            int result = ::setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group));
            assert(0 == result);

            return *this;
        }

        udp_socket& disable_loopback()
        {
            char loopch = 0;
            int result = ::setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_LOOP, &loopch, sizeof(loopch));
            assert(0 == result);

            return *this;
        }

        udp_socket& bind_interface(const char* interfaceAddress)
        {
            in_addr localInterface;
            localInterface.s_addr = inet_addr(interfaceAddress);

            int result = ::setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, sizeof(localInterface));
            assert(0 == result);

            return *this;
        }
    };
};

#endif