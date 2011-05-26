#include "ServerSocket.hpp"
#include <cstring>
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )
#endif
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>

namespace net {

ServerSocket::ServerSocket(): sockfd(0) {
}
ServerSocket::~ServerSocket() {
	if (sockfd) close(sockfd);
}

bool ServerSocket::init(int port, bool block) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
//  bzero((char *) &serv_addr, sizeof(serv_addr));
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
#ifndef WIN32
	int yes=1;
#else
	char yes=1;
#endif
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("setsockopt fail");
		return 0;
	}
	if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		return 0;
	}
	if (listen(fd,5)) {
		perror("Error on calling listen");
		return 0;
	}

	if (!block) {
#ifndef WIN32
		fcntl(fd, F_SETFL, O_NONBLOCK);
#else
		unsigned long x=1;
		ioctlsocket(fd, FIONBIO, &x);
		char flag=1;
		if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))<0) {
			perror("setsockopt TCP_NODELAY");
			return 0;
		}
#endif
	}

	sockfd = fd;

//	std::cout<<"listening port "<<port<<'\n';

	return 1;
}

void ServerSocket::pollConnections() {
	while(true) {
		sockaddr_in cli_addr;
		int clilen=sizeof cli_addr;
#ifndef WIN32
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
#else
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
#endif
		if (newsockfd < 0) break;
#ifndef WIN32
		fcntl(newsockfd, F_SETFL, O_NONBLOCK);
#else
		unsigned long x=1;
		ioctlsocket(sockfd, FIONBIO, &x);
#endif
		Connection* c = new Connection();
		c->fd = newsockfd;
		conns.push_back(c);
	}

}

void ServerSocket::readSockets() {
	for(size_t i=0; i<conns.size(); ++i) {
		while(conns[i]->read());
		if (errno!=EAGAIN) {
			delete conns[i];
			conns[i] = conns.back();
			conns.pop_back();
			--i;
		}
	}
}
void ServerSocket::send(const void* data, int n) {
	for(size_t i=0; i<conns.size(); ++i)
		conns[i]->write(data, n);
}
void ServerSocket::flush() {
	for(size_t i=0; i<conns.size(); ++i)
		conns[i]->flush();
}

} // namespace net
