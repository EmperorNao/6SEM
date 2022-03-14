#ifndef WINSOCK_ADAPTATION_H
#define WINSOCK_ADAPTATION_H


#include <stdio.h>

#define WIN32_LEAN_AND_MEAN

#ifndef TargetSocket
#define TargetSocket 8
#endif // !TargetSocket


#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <WSipx.h>

#include <stdio.h>
#include <process.h>

#include <NspAPI.h>
#include <nspapi.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Nspapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "Ws2_32.lib")


enum connType {IPX, SPX};
enum executionType {WSAOk, WSAFailed};
enum appType {Server, Client};


int ws_clean() {

    return WSACleanup();

}


void ws_fast_exit() {
    ws_clean();
    system("pause");
    exit(0);
}


int ws_start() {

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return WSAFailed;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("Could not find a usable version of Winsock.dll\n");
        ws_clean();
        return WSAFailed;
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");

    return WSAOk;

}


SOCKET WSAAPI ws_create_socket(enum connType type) {

    if (type == SPX)
        return socket(AF_IPX, SOCK_SEQPACKET, NSPROTO_SPX);
    else
        return socket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX);

}


int ws_close_socket(SOCKET s) {

    return closesocket(s);

}


int ws_bind_socket(SOCKET WSAAPI socket_no) {

    struct sockaddr_ipx s;
    //unsigned long addr = inet_addr("127.0.0.1");
    unsigned long port = 0xFFFFFFFFFFFF;

    s.sa_family = AF_IPX;

    s.sa_netnum[0] = 0;
    s.sa_netnum[1] = 0;
    s.sa_netnum[2] = 0;
    s.sa_netnum[3] = 0;

    s.sa_nodenum[0] = 255;
    s.sa_nodenum[1] = 255;
    s.sa_nodenum[2] = 255;
    s.sa_nodenum[3] = 255;
    s.sa_nodenum[4] = 255;
    s.sa_nodenum[5] = 255;
    s.sa_socket = 8;

    return bind(socket_no, (struct sockaddr*)&s, sizeof(s));

}


int ws_listen(SOCKET s) {

    return listen(s, SOMAXCONN);

}


int ws_connect(SOCKET s) {




}


int ws_accept() {



}


#endif // !WINSOCK_ADAPTATION_H