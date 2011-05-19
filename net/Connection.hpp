#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <vector>

namespace net {

struct DataWriter;
struct Connection {
	Connection(): buf(new char[1<<20]), cur(0), need(0), fd(-1), id(-1) {}
	~Connection();
#if 0
	Connection(Connection&& c):
		buf(c.buf), cur(c.cur), need(c.need), fd(c.fd),
		obuf(std::move(c.obuf))
	{
		c.buf=0;
		c.fd=0;
	}
#endif
	void close();
	bool open(const char* host, int port);
	bool read();
	void write(const void* s, int n);
	void write(DataWriter& w);
	void send(const void* s, int n) { write(s,n); }
	void send(DataWriter& w) { write(w); }
	void flush();

	char* buf;
	int cur;
	int need;
	int fd;
	std::vector<char> obuf;

	int id;

private:
	Connection(const Connection&);

	bool readSingle();
};

} // namespace net

#endif
