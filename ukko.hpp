#ifndef UKKO_UKKO_HPP
#define UKKO_UKKO_HPP

#pragma once
#include "master.hpp"
#include "Call.hpp"

namespace ukko {

#if 0
template<class T>
struct Future {
	T operator()() {
	}
};
#endif

template<class F, class...A>
void call(F&& f, A&&... args) {
	deployCall(Call(f,args...));
}

void init();

}

#endif
