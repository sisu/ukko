#ifndef DATAREADER_HPP
#define DATAREADER_HPP

#include <cstring>

namespace net {

struct DataReader {
	DataReader(void* c): cur((char*)c) { }
	char* cur;

#define F(name,type) type name() { type r = *(type*)cur; cur+=sizeof(type); return r; }
//    F(readVec2,Vec2)
	F(getInt,int)
	F(getByte,char)
	F(getDouble,double)
	F(getFloat,float)
#undef F
	void read(void* out, int n) {
		memcpy(out, cur, n);
		cur += n;
	}
};

} // namespace net

#endif
