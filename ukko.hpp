#pragma once
#include "master.hpp"
#include "Call.hpp"

namespace ukko {

template<class F, class...A>
void call(F&& f, A&&... args) {
	deployCall(Call(f,args...));
}

void init();

}
