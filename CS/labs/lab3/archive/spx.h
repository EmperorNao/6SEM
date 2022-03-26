#ifndef SPX_H
#define SPX_H


#include "winsock_adaptation.h"
#include <stdlib.h>
#include "ipx.h"
#include <time.h>


#include "windows.h"


int GetTimeMs64()
{

    /* Windows */
    FILETIME ft;
    LARGE_INTEGER li;
    LONGLONG ret = 0;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    ret = li.QuadPart;
    ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

    return ret;

}



int _spx_send_file(SOCKET s, char* filename) {

    FILE* file;
    unsigned ret_value;
    long nbytes;
    long nsend;
    struct sockaddr_ipx recvaddr;
    char SendBuf[1024] = { "Simple message" };
    int BufLen = 1024;
    long i = 0;
    HANDLE file_handler;
    long some_number = 0;

    time_t rawtime;
    struct tm* timeinfo;


    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s\n", ctime(&rawtime));
    printf("Current local time and date: %i\n", GetTimeMs64());


    //file = fopen(filename, "rb");
    printf("Trying to open file %s\n", filename);
    file_handler = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);

    if (file_handler == NULL) {

        printf("Failed to open file\n");
        return WSAFailed;

    }

    //if (!file) {
    //    printf("Failed to open file\n");
    //    return WSAFailed;
    //}

    //SetFilePointer(file_handler, 0, NULL, FILE_END);
    //fseek(file, 0L, SEEK_END);
    //nbytes = ftell(file);
    nbytes = GetFileSize(file_handler, NULL);
    //fseek(file, 0L, SEEK_SET);
    //SetFilePointer(file_handler, 0, NULL, FILE_BEGIN);

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
    ret_value = send(s, SendBuf, 1024, 0);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in sendto: %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    sprintf(SendBuf, "%ld", nbytes);
    ret_value = send(s, SendBuf, 1024, 0);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in sendto: %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    printf("Gonna send %ld packets\n", nsend);
    for (i = 0; i < nsend; ++i) {

        //fread(SendBuf, 1024, 1, file);
        ret_value = ReadFile(file_handler, SendBuf, 1024, &some_number, NULL);
        if (!ret_value) {

            printf("Error during reading from file\n");
            ws_fast_exit();

        }
        printf("Reading file %i\n", i);



        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("Current local time and date: %s\n", ctime(&rawtime));
        printf("Current local time and date: %i\n", GetTimeMs64());
        ret_value = send(s, SendBuf, 1024, 0);
        if (ret_value == SOCKET_ERROR) {
            printf("Error in sendto: %d\n", WSAGetLastError());
            ws_fast_exit();
        }

    }

    return WSAOk;

}


int _spx_recieve_file(SOCKET s, char* filename) {

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

    time_t rawtime;
    struct tm* timeinfo;



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


    ret_value = recv(s, SendBuf, BufLen, 0);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in recvfrom: %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    nsend = atol(SendBuf);

    printf("Sender Addr: \n");

    ret_value = recv(s, SendBuf, BufLen, 0);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in recvfrom: %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    nbytes = atol(SendBuf);


    printf("Expecting %ld packets with %ld bytes in it\n", nsend, nbytes);

    for (i = 0; i < nsend; ++i) {

        ret_value = recv(s, SendBuf, BufLen, 0);
        if (ret_value == SOCKET_ERROR) {
            printf("Error in recvfrom: %d\n", WSAGetLastError());
            ws_fast_exit();
        }
        fwrite(SendBuf, BufLen, 1, file);
        printf("Recieved %ld\n", i);
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("Current local time and date: %s\n", asctime(timeinfo));
        printf("Current local time and date: %i\n", GetTimeMs64());

    }

    fclose(file);

    return WSAOk;

}


struct spx_thread_data {

    SOCKET socket;
    char filename[100];
    int len;

};


DWORD WINAPI spx_file_sending_thread(LPVOID lpParam) {

    SOCKET s = ((struct spx_thread_data*)lpParam)->socket;
    //((struct spx_thread_data*)lpParam)->filename[((struct spx_thread_data*)lpParam)->filename_length] = 0;
    ((struct spx_thread_data*)lpParam)->filename[((struct spx_thread_data*)lpParam)->len] = 0;
   printf("Starting new thread with socket %i and filename %s\n", s, ((struct spx_thread_data*)lpParam)->filename);
    (struct spx_thread_data*)lpParam;
    _spx_send_file(s, ((struct spx_thread_data*)lpParam)->filename);
    printf("Ended thread with socket %i\n", s);
    //ws_close_socket(s);
    //free(lpParam);
    return 0;

}


void _spx_server() {

    unsigned ret_value;
    SOCKET WSAAPI s;
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    SOCKET new_socket;
    struct sockaddr_ipx recvaddr;
    struct sockaddr addr;
    int addrlen = sizeof(addr);
    int recvlen = sizeof(recvaddr);
    int i = 0;
    char filename[100];
    struct spx_thread_data* data;
    DWORD thread_id;
    HANDLE thread_handle;


    printf("Enter filename to send: \n");
    scanf("%s", filename);
    printf("OK. Sendind file with name %s\n", filename);


    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Started WSA\n");


    s = ws_create_socket(SPX);
    if (!s) {
        printf("Error in create socket %X\n", s);
        ws_fast_exit();
    }
    printf("Created socket\n");


    ret_value = ws_bind_socket(s);
    if (ret_value) {
        printf("Error in bind socket %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Binded socket\n");


    ret_value = getsockname(s, &addr, &addrlen);
    if (ret_value == SOCKET_ERROR) {
        printf("Error in getsockname %d\n", WSAGetLastError());
        ws_fast_exit();
    }

    printf("Addr Data:\n");
    for (i = 0; i < 14; ++i) {
        printf("%hhx ", addr.sa_data[i]);
    }
    printf("\n");

    printf("Starting to listen on socket\n");
    ret_value = listen(s, SOMAXCONN);
    if (ret_value) {
        printf("Error in listening %X\n", ret_value);
        ws_fast_exit();
    }

    ipx_send_echo();


    while (1) {


        printf("Accepting\n");
        ret_value = accept(s, (struct sockaddr*)&recvaddr, &recvlen);
        if (ret_value == SOCKET_ERROR) {
            printf("Error in accepting %d\n", WSAGetLastError());
            ws_fast_exit();
        }
        printf("Ended acception\n");

        data = (struct spx_thread_data*)malloc(sizeof(data));
        for (i = 0; i < 100; ++i) {
            data->filename[i] = filename[i];
        }
        printf("DATA->filename AFTER COPY %s\n", data->filename);

        new_socket = (SOCKET)ret_value;
        data->socket = new_socket;
        data->len = strlen(filename);

        printf("NEW SOCKET NUMBER %i\n", new_socket);

        thread_handle = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            spx_file_sending_thread,// thread function name
            data,          // argument to thread function
            0,                      // use default creation flags
            &thread_id);   // returns the thread identifier

        if (thread_handle == NULL) {

            printf("Error during trying to create thread\n");
            ws_close_socket(new_socket);
            ws_fast_exit();

        }
        printf("Created new thread with id %i\n", thread_id);
        //printf("Sending filename with new_socket\n");
        //_spx_send_file(new_socket, filename);
        //ws_close_socket(new_socket);

    }

    ws_close_socket(s);
    ws_fast_exit();

}


void _spx_client() {

    unsigned ret_value;
    SOCKET WSAAPI s;
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    struct sockaddr_ipx recvaddr;
    int recvlen = 0;
    int i = 0;

    struct sockaddr saServer;
    char filename[100];


    printf("Enter filename to save: \n");
    scanf("%s", filename);



    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Started WSA\n");


    s = ws_create_socket(SPX);
    if (!s) {
        printf("Error in create socket %X\n", s);
        ws_fast_exit();
    }
    printf("Created socket\n");


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
    recvaddr.sa_nodenum[0] = 0;
    recvaddr.sa_nodenum[1] = 0;
    recvaddr.sa_nodenum[2] = 0;
    recvaddr.sa_nodenum[3] = 0;
    recvaddr.sa_nodenum[4] = 0;
    recvaddr.sa_nodenum[5] = 0;
    recvaddr.sa_socket = TargetSocket;

    saServer = ipx_get_addr();

    printf("Addres: \n");
    for (i = 0; i < 14; ++i) {
        printf("%hhx ", saServer.sa_data[i]);
    }
    printf("\n");

    printf("Starting to connect\n");
    ret_value = connect(s, (struct sockaddr*)&saServer, sizeof(saServer));
    if (ret_value) {
        printf("Error in connecting %d\n", WSAGetLastError());
        ws_fast_exit();
    }
    printf("Connected\n");


    printf("Recieving file after connection\n");
    _spx_recieve_file(s, filename);


    ws_close_socket(s);
    ws_fast_exit();


}



void spx_app(enum appType apptype) {

    if (apptype == Server)
        _spx_server();
    else
        _spx_client();

}

#endif // !SHP_H
