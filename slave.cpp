#include "Call.hpp"
#include "serialize.hpp"
#include "net/Connection.hpp"
#include "net/DataWriter.hpp"
#include <cstdlib>
#include <iostream>
#include <cstdio>
using namespace std;

namespace ukko {

struct UkkoSlave {
	UkkoSlave() {
		char* host = getenv("UKKO_MASTER_HOST");
		char* port = getenv("UKKO_MASTER_PORT");
		if (!host || !port) return;
		bool ok = parConn.open(host, atoi(port));
		if (!ok) {
			perror("ukko: failed connecting to remote host");
			abort();
		}

		waitCommand();
	}

	void waitCommand() {
		bool ok = parConn.read();
		if (!ok) {
			perror("ukko: waiting for command failed");
			abort();
		}
		Deserializer dser(parConn.buf);
		Call c;
		serialize(dser, c);
	}
	net::Connection parConn;
} ukkoSlave;

}
