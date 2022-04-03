#pragma once
#include "ConnectionManager.hpp"


class Server {

public:
	
	static void run(std::string file) {

		std::shared_ptr<ISendable> obj = std::make_shared<FileSendable>(file);
		ConnectionManager(ConnType::IP).listening_and_sending(obj);

	}


	static void run_without_connections(std::string file) {

		std::shared_ptr<ISendable> obj = std::make_shared<FileSendable>(file);
		ConnectionManager(ConnType::UDP).listening_and_sending(obj);

	}


};