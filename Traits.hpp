#pragma once

#include <tuple>

namespace ukko {

template<class T> struct Traits;

template<class R, class...A>
struct Traits<R(A...)> {
	typedef R ret;
	typedef std::tuple<A...> args;
};
template<class R, class...A>
struct Traits<R(&)(A...)> {
	typedef R ret;
	typedef std::tuple<A...> args;
};

}
