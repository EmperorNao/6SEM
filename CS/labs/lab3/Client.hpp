#pragma once
#include "ConnectionManager.hpp"


class Client {

public:
	
	static void run(std::string file, std::string server_addr, int server_port) {

		ConnectionManager(ConnType::TCP).recieve(file, server_addr, server_port);

	}

};