#pragma once
#include <winsock2.h>

enum class ConnType { UDP, IP, TCP };

class Socket {

private:
    SOCKET socket_descriptor = INVALID_SOCKET;

public:

    Socket(SOCKET s): socket_descriptor(s) {};
    Socket(enum class ConnType sockType = ConnType::UDP, bool listening = true, bool for_all = false, USHORT port = 27015);


    bool is_valid() {
        return socket_descriptor != INVALID_SOCKET;
    }

    SOCKET get_descriptor() {
        return socket_descriptor;
    }

    ~Socket() {
        closesocket(socket_descriptor);
    }

};