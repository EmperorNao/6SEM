 #ifndef IPX_H
#define IPX_H

#include "winsock_adaptation.h"
#include <stdlib.h>


#ifndef IPX_SOCKET_NO
#define IPX_SOCKET_NO 8
#endif // !IPX_SOCKET_NO


#ifndef MAX_FILE_LENGTH
#define MAX_FILE_LENGTH 5000000
#endif // !MAX_FILE_LENGTH


typedef struct {
    unsigned int Checksum;
    unsigned int Length;
    unsigned char TransportControl;
    unsigned char PacketType;
    unsigned char DestantionNetwork[4];
    unsigned char DestantionNode[6];
    unsigned int DestantionSocket;
    unsigned char SourceNetwork[4];
    unsigned char SourceNode[6];
    unsigned int SourceSocket;
} IPXHeader;


typedef struct {
    void far* LinkAddress;
    void far* ESRAddress;
    unsigned char InUseFlag;
    unsigned char CompletionCode;
    unsigned int SocketNumber;
    unsigned char IPXWorkspace[4];
    unsigned char DriverWorkspace[12];
    unsigned char ImmediateAddress[6];
    unsigned int FragmentCount;
    void far* FragmentAddress_1;
    unsigned int FragmentSize_1;
    void far* FragmentAddress_2;
    unsigned int FragmentSize_2;
} ECB_2;


int _ipx_send_file(SOCKET s, char* filename) {

    FILE* file;
    unsigned ret_value;
    long nbytes;
    long nsend;
    struct sockaddr_ipx recvaddr;
    char SendBuf[1024] = { "Simple message" };
    int BufLen = 1024;
    long i = 0;


    file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file\n");
        return WSAFailed;
    }


    fseek(file, 0L, SEEK_END);
    nbytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    nsend = nbytes / 1024;
    if (nbytes % 1024 != 0)
        nsend += 1;


    recvaddr.sa_family = AF_IPX;
    recvaddr.sa_netnum[0] = 0;
    recvaddr.sa_netnum[1] = 0;
    recvaddr.sa_netnum[2] = 0;
    recvaddr.sa_netnum[3] = 0;
    recvaddr.sa_nodenum[0] = 255;
    recvaddr.sa_nodenum[1] = 255;
    recvaddr.sa_nodenum[2] = 255;
    recvaddr.sa_nodenum[3] = 255;
    recvaddr.sa_nodenum[4] = 255;
    recvaddr.sa_nodenum[5] = 255;
    recvaddr.sa_socket = IPX_SOCKET_NO;


    sprintf(SendBuf, "%ld", nsend);
    ret_value = sendto(s, SendBuf, 1024, 0, (struct sockaddr*)&recvaddr, sizeof(recvaddr));
    if (ret_value == SOCKET_ERROR) {
        printf("Error in sendto: %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    sprintf(SendBuf, "%ld", nbytes);
    ret_value = sendto(s, SendBuf, 1024, 0, (struct sockaddr*)&recvaddr, sizeof(recvaddr));
    if (ret_value == SOCKET_ERROR) {
        printf("Error in sendto: %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    printf("Gonna send %ld packets\n", nsend);
    for (i = 0; i < nsend; ++i) {

        fread(SendBuf, 1024, 1, file);
        ret_value = sendto(s, SendBuf, 1024, 0, (struct sockaddr*)&recvaddr, sizeof(recvaddr));
        if (ret_value == SOCKET_ERROR) {
            printf("Error in sendto: %d\n", WSAGetLastError());
            ws_fast_exit();
        }

    }

    return WSAOk;

}


int _ipx_recieve_file(SOCKET s, char* filename) {

    FILE* file;
    unsigned ret_value;
    long nbytes;
    long nsend;
    struct sockaddr_ipx recvaddr;
    char SendBuf[1024];
    int BufLen = 1024;
    long i = 0;
    long iter = 0;
    struct sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);



    file = fopen(filename, "wb+");
    if (!file) {
        printf("Failed to create file\n");
        return WSAFailed;
    }
    fseek(file, MAX_FILE_LENGTH, SEEK_SET);

    fseek(file, 0, SEEK_SET);


    recvaddr.sa_family = AF_IPX;
    recvaddr.sa_netnum[0] = 0;
    recvaddr.sa_netnum[1] = 0;
    recvaddr.sa_netnum[2] = 0;
    recvaddr.sa_netnum[3] = 0;
    recvaddr.sa_nodenum[0] = 255;
    recvaddr.sa_nodenum[1] = 255;
    recvaddr.sa_nodenum[2] = 255;
    recvaddr.sa_nodenum[3] = 255;
    recvaddr.sa_nodenum[4] = 255;
    recvaddr.sa_nodenum[5] = 255;
    recvaddr.sa_socket = IPX_SOCKET_NO;


    ret_value = recvfrom(s, SendBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in recvfrom: %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    nsend = atol(SendBuf);

    printf("Sender Addr: \n");

    ret_value = recvfrom(s, SendBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in recvfrom: %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    nbytes = atol(SendBuf);


    printf("Expecting %ld packets with %ld bytes in it\n", nsend, nbytes);

    for (i = 0; i < nsend; ++i) {

        ret_value = recvfrom(s, SendBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
        if (ret_value == SOCKET_ERROR) {
            printf("Error in recvfrom: %d\n", WSAGetLastError());
            ws_fast_exit();
        }
        fwrite(SendBuf, BufLen, 1, file);
        printf("Recieved %ld\n", i);

    }

    fclose(file);

    return WSAOk;

}



void _ipx_server() {


    unsigned ret_value;
    SOCKET WSAAPI s;
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    char filename[100];


    printf("Enter filename to send: \n");
    scanf("%s", filename);

    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Started WSA\n");


    s = ws_create_socket(IPX);
    if (!s) {
        printf("Error in create socket %X\n", s);
        ws_fast_exit();
    }
    printf("Created socket\n");

    ret_value = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
    if (ret_value) {
        printf("Error in set sock option %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    ret_value = ws_bind_socket(s);
    if (ret_value) {
        printf("Error in bind socket %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Binded socket\n");

    _ipx_send_file(s, filename);

    ws_close_socket(s);
    ws_fast_exit();

}



int _ipx_client() {

    unsigned ret_value;
    SOCKET WSAAPI s;
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    struct sockaddr_ipx recvaddr;

    struct sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);


    char filename[100];

    printf("Enter filename to save: \n");
    scanf("%s", filename);


    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Started WSA\n");


    s = ws_create_socket(IPX);
    if (!s) {
        printf("Error in create socket %X\n", s);
        ws_fast_exit();
    }
    printf("Created socket\n");

    ret_value = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
    if (ret_value) {
        printf("Error in set sock option %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    ret_value = ws_bind_socket(s);
    if (ret_value) {
        printf("Error in bind socket %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Binded socket\n");

    recvaddr.sa_family = AF_IPX;
    recvaddr.sa_netnum[0] = 0;
    recvaddr.sa_netnum[1] = 0;
    recvaddr.sa_netnum[2] = 0;
    recvaddr.sa_netnum[3] = 0;

    recvaddr.sa_nodenum[0] = 255;
    recvaddr.sa_nodenum[1] = 255;
    recvaddr.sa_nodenum[2] = 255;
    recvaddr.sa_nodenum[3] = 255;
    recvaddr.sa_nodenum[4] = 255;
    recvaddr.sa_nodenum[5] = 255;
    recvaddr.sa_socket = IPX_SOCKET_NO;


    printf("Waiting a datagram from sender...\n");
    _ipx_recieve_file(s, filename);


    ws_close_socket(s);
    ws_fast_exit();

}


void ipx_send_echo() {

    unsigned ret_value;
    SOCKET WSAAPI s;
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    struct sockaddr_ipx recvaddr;
    char SendBuf[1024] = { "Simple message" };
    int BufLen = 1024;
    long i = 0;


    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }


    s = ws_create_socket(IPX);
    if (!s) {
        printf("Error in create socket %X\n", s);
        ws_fast_exit();
    }

    ret_value = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
    if (ret_value) {
        printf("Error in set sock option %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    ret_value = ws_bind_socket(s);
    if (ret_value) {
        printf("Error in bind socket %X\n", ret_value);
        ws_fast_exit();
    }

    recvaddr.sa_family = AF_IPX;
    recvaddr.sa_netnum[0] = 0;
    recvaddr.sa_netnum[1] = 0;
    recvaddr.sa_netnum[2] = 0;
    recvaddr.sa_netnum[3] = 0;
    recvaddr.sa_nodenum[0] = 255;
    recvaddr.sa_nodenum[1] = 255;
    recvaddr.sa_nodenum[2] = 255;
    recvaddr.sa_nodenum[3] = 255;
    recvaddr.sa_nodenum[4] = 255;
    recvaddr.sa_nodenum[5] = 255;
    recvaddr.sa_socket = IPX_SOCKET_NO;

    ret_value = sendto(s, SendBuf, 1024, 0, (struct sockaddr*)&recvaddr, sizeof(recvaddr));
    if (ret_value == SOCKET_ERROR) {
        printf("Error in sendto: %d\n", WSAGetLastError());
        ws_close_socket(s);
        ws_clean();
        return;
    }

    ws_close_socket(s);

}


struct sockaddr ipx_get_addr() {

    unsigned ret_value;
    SOCKET WSAAPI s;
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    struct sockaddr_ipx recvaddr;

    char SendBuf[1024];
    int BufLen = 1024;
    long i = 0;
    long iter = 0;

    struct sockaddr SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);


    printf("Waiting for echo message with address\n");

    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }


    s = ws_create_socket(IPX);
    if (!s) {
        printf("Error in create socket %X\n", s);
        ws_fast_exit();
    }

    ret_value = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
    if (ret_value) {
        printf("Error in set sock option %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    ret_value = ws_bind_socket(s);
    if (ret_value) {
        printf("Error in bind socket %X\n", ret_value);
        ws_fast_exit();
    }

    ret_value = recvfrom(s, SendBuf, BufLen, 0, &SenderAddr, &SenderAddrSize);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in recvfrom: %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    printf("Sender Addr: \n");

    ws_close_socket(s);
    return SenderAddr;

}



void ipx_app(enum appType apptype) {

    if (apptype == Server)
        _ipx_server();
    else
        _ipx_client();

}


#endif // !IPX_H
