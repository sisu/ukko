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

void ServerSocket::init(int port, bool block) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

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
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("setsockopt fail");
		exit(3);
	}
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	if (listen(sockfd,5)) {
		perror("Error on calling listen");
		exit(5);
	}

	if (!block) {
#ifndef WIN32
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
#else
		unsigned long x=1;
		ioctlsocket(sockfd, FIONBIO, &x);
		char flag=1;
		if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))<0) {
			perror("setsockopt TCP_NODELAY");
			exit(17);
		}
#endif
	}

	std::cout<<"listening port "<<port<<'\n';
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
