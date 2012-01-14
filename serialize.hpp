#ifndef UKKO_SERIALIZE_HPP
#define UKKO_SERIALIZE_HPP

#pragma once
#include <cstring>
#include <cstdint>

namespace ukko {

struct Serializer {
	std::vector<char> buf;
	template<class T>
	Serializer& operator&(T& t) {
		serialize(*this, t);
		return *this;
	}
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
inline void serialize(S& s, T& t) {
	t.serialize(s);
}

template<class S>
inline void serialize(S& s, Serializer& ss) {
	s & ss.buf;
}

#define FORALL_PRIMITIVES(F)\
F(int8_t); F(int16_t); F(int32_t); F(int64_t);\
F(uint8_t); F(uint16_t); F(uint32_t); F(uint64_t);\
F(float); F(double); F(long double);\
F(char);

#define S(type)\
	inline void serialize(Serializer& s, type& t) {\
		char* ct = (char*)&t;\
		s.buf.insert(s.buf.begin(), ct, ct+sizeof(type));\
	}
FORALL_PRIMITIVES(S)
#undef S

#define S(type)\
	inline void serialize(Deserializer& s, type& t) {\
		t = *(type*)s.buf;\
		s.buf += sizeof(type);\
	}
FORALL_PRIMITIVES(S)
#undef S

#undef FORALL_PRIMITIVES

#if 0
inline void serialize(Deserializer& s, char& c) {
}
#endif

template<class T>
inline void serialize(Serializer& s, std::vector<T>& v) {
	s & v.size();
	for(size_t i=0; i<v.size(); ++i)
		s & v[i];
}
template<class T>
inline void serialize(Deserializer& s, std::vector<T>& v) {
	size_t size;
	s & size;
	v.resize(size);
	for(size_t i=0; i<size; ++i)
		s & v[i];
}

}

#endif
