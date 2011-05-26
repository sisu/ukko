#include "master.hpp"
#include "net/ServerSocket.hpp"

namespace ukko {

void deployCall(Call&& c) {
	net::ServerSocket srv;
	int port = 43752;
	if (!srv.init(port, 1)) {
		perror("ukko: server init failed");
		exit(-1);
	}
}

}
