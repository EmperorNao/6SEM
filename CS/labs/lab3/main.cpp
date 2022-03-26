#include "ConnectionManager.hpp"


int main() {




    //ConnectionManager manager;
    //Socket s;
    //if (s.is_valid()) {
    //    std::cout << "socket is ok" << std::endl;
    //}
    try {

        /*Socket sforall(ConnType::UDP, true);*/
        int type = 0;
        std::cout << "Server/Client (1/0)" << std::endl;
        std::cin >> type;
        //std::cout <<
        ConnectionManager manager;
        if (type) {
            std::shared_ptr<ISendable> obj = std::make_shared<FileSendable>("P:\\D\\6SEM\\CS\\labs\\lab3\\ConnectionManager.hpp");
            //manager.recieve();
            //manager.send(obj, "127.0.0.1", 27015);
            manager.listening_and_sending(obj);
            //manager.connect("127.0.0.1", 8080);
            //manager.send(obj, {}, /*send_for_all*/true);
        }
        else {
            manager.recieve_some("outp_filename", "127.0.0.1", 27015);
        }


        WSADATA data;
        char name[100];
        int namelen;
        //std::cout << "Name is " << host->h_name << std::endl;

        struct in_addr addr;
        auto addresses = manager.get_local_addresses();
        for (auto&& addr_val : addresses) {

            addr.s_addr = addr_val;
            std::cout << inet_ntoa(addr) << std::endl;

        }

        //std::cout << "Readed" << std::endl;

    }
    catch(std::exception & e) {

        std::cout << e.what() << std::endl;

    }

    system("pause");

}
