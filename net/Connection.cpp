#include "Connection.hpp"
#include "DataWriter.hpp"
#include <sys/types.h>
#include <fcntl.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )
#endif

#include <iostream>
#include <cassert>
using namespace std;

namespace net {

Connection::~Connection() {
	close();
	delete[] buf;
}
void Connection::close() {
// FIXME: fix me
#ifndef WIN32
	if (fd>=0) ::close(fd);
#endif
	fd=-1;
}

bool Connection::open(const char* host, int port) {
	close();
	hostent* serv = gethostbyname(host);
	if (!serv) {
		cout<<"gethostbyname failed\n";
		return 0;
	}
	sockaddr_in addr;
#ifdef WIN32
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd==INVALID_SOCKET) {
		std::cout<<"binding socket failed "<<WSAGetLastError()<<endl;
		return 0;
	}
#else
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
#endif
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
#ifdef WIN32
	addr.sin_addr.s_addr = ((struct in_addr *)(serv->h_addr))->s_addr;
#else
	bcopy(serv->h_addr, &addr.sin_addr.s_addr, serv->h_length);
#endif
	addr.sin_port = htons(port);
	if (::connect(sockfd, (sockaddr*)&addr, sizeof(addr))<0) {
		perror("Connecting failed");
#ifdef WIN32
		std::cout<<"connect error: "<<WSAGetLastError()<<'\n';
#endif
		return 0;
	}
#ifndef WIN32
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
#else
	unsigned long x=1;
	ioctlsocket(sockfd, FIONBIO, &x);
	char flag=1;
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))<0) {
		perror("setsockopt TCP_NODELAY");
		return 0;
	}
#endif
	fd = sockfd;
	return 1;
}

#ifdef WIN32
int get(int fd, void* buf, int n)
{
	int r = recv(fd, (char*)buf, n, 0);
	if (r<0) {
		int rr = WSAGetLastError();
		if (rr==WSAEWOULDBLOCK) errno=EAGAIN;
		else cout<<"rect error: "<<rr<<endl;
	} //else cout<<"got data "<<r<<'\n';
	return r;
}
#else
int get(int fd, void* buf, int n)
{
	return ::read(fd, buf, n);
}
#endif

bool Connection::read()
{
    assert(cur<1<<20);
	if (cur<4) {
		int n = get(fd, buf+cur, 4-cur);
		if (n<0) return 0;
//		cout<<"got data: "<<n<<'\n';
		cur += n;
		if (cur<4) return 0;
		need = *(int*)buf;
//		cout<<"need: "<<need<<'\n';
//		assert(need < 1<<10);
	}
//	cout<<"asd "<<cur<<' '<<need-(cur-4)<<'\n';
	int n =get(fd, buf+cur, need-(cur-4));
	if (n<0) return 0;
//	cout<<"real data: "<<n<<'\n';
//	for(int i=0; i<n; ++i) cout<<hex<<(int)((char*)buf)[cur+i]<<' ';cout<<'\n';
	cur += n;
	if (cur<4+need) return 0;
	cur=0;

	return 1;
}

void Connection::write(const void* s, int n)
{
#if 1
	char* np = (char*)&n;
	obuf.insert(obuf.end(), np, np+4);
	char* ss = (char*)s;
	obuf.insert(obuf.end(), ss, ss+n);
#else
	::write(fd, &n, 4);
	::write(fd, s, n);
#endif
}
void Connection::write(DataWriter& w)
{
	write(w.data(), w.len());
}
void Connection::flush()
{
	if (obuf.size()) {
		cout<<"sending "<<obuf.size()<<" bytes\n";
#if 0
		#ifdef WIN32
		int r = send(fd, &obuf[0], obuf.size(), 0);
		if (r != obuf.size()) {
			cout<<"failed send "<<WSAGetLastError()<<endl;
		}
		#else
		(void)::write(fd, &obuf[0], obuf.size());
		#endif
#else
		int r = ::send(fd, &obuf[0], obuf.size(), 0);
		if (r != (int)obuf.size()) {
#ifdef WIN32
			cout<<"failed sending "<<WSAGetLastError()<<endl;
#else
			perror("send failed");
#endif
		}
#endif
		obuf.clear();
	}
}

} // namespace net
