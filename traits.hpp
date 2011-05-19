#pragma once

#include <tuple>

namespace ukko {

template<class T> struct traits;

template<class R, class...A>
struct traits<R(A...)> {
	typedef R ret;
	typedef std::tuple<A...> args;
};

}
