#include "ConnectionManager.hpp"
#include "argparse/include/argparse/argparse.hpp"
#include "Server.hpp"
#include "Client.hpp"

INITIALIZE_EASYLOGGINGPP



int main(int argc, char* argv[]) {


    std::cout << "OK" << std::endl;
    argparse::ArgumentParser program("main");

    program.add_argument("--verbose")
        .help("increase output verbosity")
        .default_value(false)
        .implicit_value(true);


    program.parse_args(argc, argv);


    if (program["--verbose"] == true) {

        std::cout << "verbose is in" << std::endl;

    }


    try {

        int type = 0;
        std::cout << "Server/Client (1/0)" << std::endl;
        std::cin >> type;
        ConnectionManager manager;
        if (type) {
            el::Configurations conf("P:\\D\\6SEM\\CS\\labs\\lab3\\serverlogger");
            el::Loggers::reconfigureAllLoggers(conf);
            Server::run("p:\\D\\Pictures\\My\\Yandex.png");
        }
        else {
            el::Configurations conf("P:\\D\\6SEM\\CS\\labs\\lab3\\clientlogger");
            el::Loggers::reconfigureAllLoggers(conf);
            Client::run("MyPhoto.png", "127.0.0.1", 27015);
        }

    }
    catch(std::exception & e) {

        std::cout << e.what() << std::endl;

    }

    system("pause");


}
