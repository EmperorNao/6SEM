#pragma once
#include "ConnectionManager.hpp"


class Client {


	void run(std::string server_addr, int server_port) {

		ConnectionManager::send(std::make_shared<StringSendable>("SendMeFilePls"));


	}

};