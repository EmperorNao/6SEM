#include "Socket.hpp"
#include "ConnectionManager.hpp"


Socket::Socket(enum class ConnType sockType, bool listening, bool for_all, USHORT port) {

    if (sockType == ConnType::UDP) {
        socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    else if (sockType == ConnType::IP) {
        socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    }
    else {
        throw std::invalid_argument("Wrong connection type");
    }


    if (socket_descriptor == INVALID_SOCKET) {
        throw std::domain_error("Error in socket creation " + std::to_string(WSAGetLastError()));
    }

    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    service.sin_port = htons(port);


    if (for_all) {

        BOOL bOptVal = TRUE;
        int bOptLen = sizeof(BOOL);
        int ret_value = setsockopt(socket_descriptor, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
        if (ret_value == SOCKET_ERROR) {
            throw std::domain_error("Error in setting broadcasting" + std::to_string(WSAGetLastError()));
        }

    }

    if (listening && bind(socket_descriptor, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        throw std::domain_error("Error in socket binding" + std::to_string(WSAGetLastError()));
    }

}