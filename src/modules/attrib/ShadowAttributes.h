/**
 * @file
 */

#pragma once

#include "Container.h"

#ifdef _WIN32
#undef max
#endif

namespace attrib {

/**
 * @brief ShadowAttributes can be used on the client side to just shadow the server side state.
 *
 * There is no max calculation done here, but just taken from the server. This class is not thread safe.
 *
 * @sa Attributes
 * @ingroup Attributes
 */
class ShadowAttributes {
protected:
	Values _current;
	Values _max;
public:
	bool update(double /*deltaFrameSeconds*/) {
		return true;
	}

	inline void setMax(Type type, double value) {
		_max.put(type, value);
	}

	double setCurrent(Type type, double value) {
		_current.put(type, value);
		return value;
	}

	double current(Type type) const {
		auto i = _current.find(type);
		if (i == _current.end()) {
			return 0.0;
		}
		return i->value;
	}

	double max(Type type) const {
		auto i = _max.find(type);
		if (i == _max.end()) {
			return 0.0;
		}
		return i->value;
	}
};

}
