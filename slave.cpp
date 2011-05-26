#include "Call.hpp"
#include "serialize.hpp"
#include "net/Connection.hpp"
#include "net/DataWriter.hpp"
#include <cstdlib>
#include <iostream>
#include <cstdio>
using namespace std;

namespace ukko {

void waitCommand(net::Connection& conn) {
	bool ok = conn.read();
	if (!ok) {
		perror("ukko: waiting for command failed");
		abort();
	}
	Deserializer dser(conn.buf);
	Call c;
	serialize(dser, c);
}

void runSlave(const char* host, int port) {
	net::Connection conn;
	bool ok = conn.open(host, port);
	if (!ok) {
		perror("ukko: failed connecting to remote host");
		abort();
	}

	waitCommand(conn);
}

}
