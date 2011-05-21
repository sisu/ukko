#pragma once
#include <vector>
#include <cstddef>
#include <type_traits>
#include "serialize.hpp"
#include "Traits.hpp"

namespace ukko {

template<class T>
void buildObject(void* out, Serializer& s) {
	typedef typename std::remove_reference<T>::type U;
	U* t = new(out) U;
	serialize(s, *t);
}

struct Call {
	void* func;
	std::vector<void*> params;

	template<class S>
	void serialize(S& s) {
		s & func & params;
	}

	Call(){}

	template<class F, class...A>
	explicit Call(F&& f, A&&... args) {
		params.reserve(sizeof...(A));
		typedef typename Traits<F>::args P;
		initParams<0,P>(std::forward<A>(args)...);
	}

	template<size_t N, class T>
	void initParams() {}

	template<size_t N, class T, class X, class...A>
	void initParams(X&& x, A&&... args) {
		// FIXME: serialize somewhere...
		// Taking pointers to variables hopefully forces instantiation...
		typedef typename std::tuple_element<N,T>::type Y;
		void(*ptr)(void*,Serializer&) = buildObject<Y>;
		params.push_back((void*)ptr);
		initParams<N+1,T>(std::forward<A>(args)...);
	}

};

}
