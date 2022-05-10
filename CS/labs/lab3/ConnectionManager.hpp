#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>


#include <winsock2.h>

#include "WSALoader.hpp"
#include "ISenadble.hpp"
#include "Socket.hpp"

#include "easyloggingpp/src/easylogging++.h"



class ConnectionManager {

private:

    std::string conn_address = "127.0.0.1";
    unsigned long port = 0;
    //Socket socket;
    static int const packet_size = 1024;
    ConnType connType;

public:

    ConnectionManager(ConnType connType_) : connType(connType_) {}

    static std::string err() {
        return std::to_string(WSAGetLastError());
    }


    static std::string get_hostname() {

        char name[100];
        if (gethostname(name, 100) == SOCKET_ERROR) {
            throw std::domain_error("Error during get host name " + err());
        }
        return std::string(name);

    }


    static std::vector<unsigned long> get_local_addresses() {

        std::vector<unsigned long> addresses;

        struct hostent* remoteHost;
        remoteHost = gethostbyname(get_hostname().c_str());

        int i = 0;
        while (remoteHost->h_addr_list[i] != 0) {
            addresses.push_back(*(u_long*)remoteHost->h_addr_list[i++]);
        }

        if (!addresses.size()) {
            throw std::domain_error("Don't found any address " + err());
        }

        return addresses;

    }


    void listening_and_sending(std::shared_ptr<ISendable>& obj) {


        if (!(connType == ConnType::IP or connType == ConnType::TCP)) {
            throw std::domain_error("Wrong conn type provided for listening_and_sending: " + std::to_string(WSAGetLastError()));
        }
        //struct sockaddr_i recvaddr;
        //struct sockaddr addr;
        //int addrlen = sizeof(addr);
        //int recvlen = sizeof(recvaddr);
        Socket main_socket(connType);
        listen(main_socket.get_descriptor(), SOMAXCONN);

        int i = 0;
        while (true) {

            struct sockaddr addr;
            int len = sizeof(addr);
            SOCKET accept_descriptor;
            accept_descriptor = accept(main_socket.get_descriptor(), &addr, &len);
            if (accept_descriptor == INVALID_SOCKET) {
                throw std::domain_error("Error in socket accept " + std::to_string(WSAGetLastError()));
            }
            std::shared_ptr<Socket> accept_socket = std::make_shared<Socket>(accept_descriptor);

            std::thread t(send_with_socket, std::ref(obj), accept_socket);
            t.detach();
            
        }


    }


    



    //struct sockaddr_in recieve(bool log=true, int port=0) {

    //    if (!(connType == ConnType::IP or connType == ConnType::TCP)) {
    //        throw std::domain_error("Wrong conn type provided for listening_and_sending: " + std::to_string(WSAGetLastError()));
    //    }

    //    Socket s(connType);
    //    char RecvBuf[packet_size];

    //    struct sockaddr_in SenderAddr;
    //    int SenderAddrSize = sizeof(SenderAddr);

    //    if (recvfrom(s.get_descriptor(), RecvBuf, packet_size, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize) == SOCKET_ERROR) {
    //        throw std::domain_error("Error in recfrom " + err());
    //    }

    //}


    static void send_with_socket(std::shared_ptr<ISendable>& obj, std::shared_ptr<Socket> s) {


        auto data = obj->split(ConnectionManager::packet_size);

        std::string size = std::to_string(data.size());

        for (auto buf : data) {

            if (send(
                s->get_descriptor(),
                reinterpret_cast<char*>(buf.data()),
                ConnectionManager::packet_size,
                0) == SOCKET_ERROR) {
                throw std::domain_error("Error during sendto " + err());
            }
            LOG(INFO) << "Sended";

        }

    }



    void recieve(std::string filename, std::string addr, USHORT port) {

        if (!(connType == ConnType::IP or connType == ConnType::TCP)) {
            throw std::domain_error("Wrong conn type provided for listening_and_sending: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in clientService;
        clientService.sin_family = AF_INET;
        clientService.sin_addr.s_addr = inet_addr(addr.c_str());
        clientService.sin_port = htons(port);

        char recvbuf[ConnectionManager::packet_size];
        Socket s(connType, false);
        char RecvBuf[packet_size];

        struct sockaddr_in SenderAddr;
        int SenderAddrSize = sizeof(SenderAddr);

        if (connect(s.get_descriptor(), (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
            throw std::domain_error("Error in rec" + err());
        }

        std::shared_ptr<FileSendable> obj = std::make_shared<FileSendable>(filename);
        int i = 0;
        while (true) {
            if (recv(s.get_descriptor(), recvbuf, ConnectionManager::packet_size, 0) <= 0) {
                break;
            }
            LOG(INFO) << "Recieved";
            obj->append(recvbuf, ConnectionManager::packet_size);
            i++;
        };
        obj->save();
    }




    int sendto_(std::shared_ptr<ISendable>& obj, sockaddr addr) {/*std::string addr, USHORT port) {*/
  
        if (!(connType == ConnType::UDP)) {
            throw std::domain_error("Wrong conn type provided for sendto_: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in RecvAddr;

        const int len = 1024;


        Socket sendSocket(connType, false);

        //RecvAddr.sin_family = AF_INET;
        //RecvAddr.sin_port = htons(port);
        //RecvAddr.sin_addr.s_addr = inet_addr(addr.c_str());


        auto data = obj->split(len);

        std::string size = std::to_string(data.size());

        for (auto buf : obj->split(len)) {

            if (sendto(
                sendSocket.get_descriptor(), 
                reinterpret_cast<char*>(buf.data()), 
                len, 
                0, 
                (const sockaddr*)&addr, 
                sizeof(RecvAddr)) == SOCKET_ERROR) {
                throw std::domain_error("Error during sendto " + err());
            }

        }

    }


    // TODO
    /*void recievefrom(std::string filename, std::shared_ptr<Socket> s) {

        if (!(connType == ConnType::IP or connType == ConnType::TCP)) {
            throw std::domain_error("Wrong conn type provided for listening_and_sending: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in clientService;
        clientService.sin_family = AF_INET;
        clientService.sin_addr.s_addr = inet_addr(addr.c_str());
        clientService.sin_port = htons(port);

        char recvbuf[ConnectionManager::packet_size];
        Socket s(connType, false);
        char RecvBuf[packet_size];

        struct sockaddr_in SenderAddr;
        int SenderAddrSize = sizeof(SenderAddr);

        if (connect(s.get_descriptor(), (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
            throw std::domain_error("Error in rec" + err());
        }

        recvfrom(s->get_descriptor(), , BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

        std::shared_ptr<FileSendable> obj = std::make_shared<FileSendable>(filename);
        int i = 0;
        while (true) {
            if (recv(s.get_descriptor(), recvbuf, ConnectionManager::packet_size, 0) <= 0) {
                break;
            }
            LOG(INFO) << "Recieved";
            obj->append(recvbuf, ConnectionManager::packet_size);
            i++;
        };
        obj->save();
    }*/

};
