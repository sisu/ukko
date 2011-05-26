#pragma once

#include "Connection.hpp"
#include <vector>
#include <iostream>

namespace net {

struct ServerSocket {
	ServerSocket();
	~ServerSocket();
	bool init(int port, bool block);
	void pollConnections();
	void readSockets();
	void send(const void* data, int n);
	void flush();

	int sockfd;
	std::vector<Connection*> conns;
};

} // namespace net
