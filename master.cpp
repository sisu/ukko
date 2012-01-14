#include "master.hpp"
#include "net/ServerSocket.hpp"
#include <string>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace ukko {

namespace {
net::ServerSocket server;

string progname() {
	pid_t pid = getpid();
	ostringstream oss;
	oss << "/proc/" << pid << "/comm";
	ifstream in(oss.str());
	string prog;
	getline(in, prog);
	char* cwd = getcwd(0,0);
	string path = cwd;
	free(cwd);
	return path + '/' + prog;
}
string getHost() {
	// TODO
	FILE* f = popen("hostname", "r");
	char buf[1024];
	fgets(buf, sizeof(buf), f);
	size_t len = strlen(buf);
	if (buf[len-1]==10) buf[len-1]=0;
	pclose(f);
	return buf;
}

void startRemote(int port) {
	// TODO
	string remote = "127.0.0.1";
	string prog = progname();
	ostringstream oss;
	oss<<"ssh \'"<<remote<<"\' ";
	oss<<"UKKO_MASTER_HOST="<<getHost();
	oss<<" UKKO_MASTER_PORT="<<port;
	oss<<" \'"<<prog<<'\'';
	string command = oss.str();
	cout<<"command: "<<command<<'\n';
	system(command.c_str());
}

};

void deployCall(Call&& c) {
	int port = 43752;
	if (server.sockfd==0 && !server.init(port, 1)) {
		perror("ukko: server init failed");
		exit(-1);
	}
	startRemote(port);
}
void initMaster() {
}

}
