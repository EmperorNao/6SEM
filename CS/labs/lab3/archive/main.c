//#define _CRT_SECURE_NO_WARNINGS
//#include "app.h"
#include "ConnectionManager.hpp"



int main() {

   /* unsigned ret_value;
    SOCKET WSAAPI s;
	printf("Hello world %X!\n", NSPROTO_IPX);

    enum connType type = IPX;

    ret_value = ws_start();
    if (ret_value) {
        printf("Error in start %X\n", ret_value);
        ws_fast_exit();
    }
    printf("Started wsa\n");


    s = ws_create_socket(type);

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

    if (type == SPX) {

        ret_value = ws_listen(s);
        if (ret_value) {
            printf("Error int listen socket %X\n", ret_value);
            ws_fast_exit();
        }

    }





    ws_close_socket(s);
    ws_fast_exit();*/


    // char c;
    // enum appType app;
    // enum connType conn;
    printf("1 - server, 0 - client | i - IPX, s - SPX\n");
    //
    // c = getchar();
    // if (c == '1')
    //     app = Server;
    // else
    //     app = Client;
    //
    // c = getchar();
    // if (c == 'i')
    //     conn = IPX;
    // else
    //     conn = SPX;
    //
    //
    // start_app(app, conn);
    try {
        ConnectionManager manager;
    }
    catch(std::exception & e) {

        std::cout << e.what() << std::endl;

    }
    system("pause");
 

}
