#pragma once
#include <winsock2.h>
#include <mutex>


namespace WSA {

    class WSALoader {


    public:

        WSALoader() {
            WSADATA data;
            if (WSAStartup(MAKEWORD(2, 2), &data) != NO_ERROR) {
                throw std::domain_error("Error in WSAStartup");
            }
        }
        ~WSALoader() { WSACleanup(); }

    };


    static WSALoader loader;

}