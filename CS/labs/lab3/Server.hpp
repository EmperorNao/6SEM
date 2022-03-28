#pragma once
#include "ConnectionManager.hpp"


class Server {

public:
	
	static void run(std::string file) {

		std::shared_ptr<ISendable> obj = std::make_shared<FileSendable>(file);
		ConnectionManager::listening_and_sending(obj);

	}

};