#pragma once
#include <vector>
#include <cstddef>

namespace ukko {

struct Call {
	ptrdiff_t func;
	std::vector<ptrdiff_t> params;

	template<class S>
	void serialize(S& s) {
		s & func & params;
	}
};

}
