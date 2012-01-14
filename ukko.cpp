#include "ukko.hpp"
#include <cstdlib>

namespace ukko {

void initMaster();

void runSlave(const char* host, int port);

bool initSlave() {
	char* host = getenv("UKKO_MASTER_HOST");
	char* port = getenv("UKKO_MASTER_PORT");
	if (!host || !port) return 0;
	runSlave(host, atoi(port));
	return 1;
}

void init() {
	initMaster();
	if (initSlave()) {
		exit(0);
	}
}

}
