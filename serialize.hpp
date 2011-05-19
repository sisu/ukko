#pragma once
#include <cstring>
#include <cstdint>

namespace ukko {

struct Serializer {
};
struct Deserializer {
	Deserializer(char* buf): buf(buf) {}
	template<class T>
	Deserializer& operator&(T& t) {
		serialize(*this, t);
		return *this;
	}
	char* buf;
};

template<class S, class T>
void serialize(S& s, T& t) {
	t.serialize(s);
}

#define S(type)\
	void serialize(Deserializer& s, type& t) {\
		t = *(type*)s.buf;\
	}
S(int8_t);
S(int16_t);
S(int32_t);
S(int64_t);
S(uint8_t);
S(uint16_t);
S(uint32_t);
S(uint64_t);
#undef S

template<class T>
void serialize(Deserializer& s, std::vector<T>& v) {
	size_t size;
	s & size;
	v.resize(size);
	for(size_t i=0; i<size; ++i) {
		s & v[i];
	}
}

}
