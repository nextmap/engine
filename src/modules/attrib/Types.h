/**
 * @file
 */

#pragma once

#include <functional>
#include <cstring>
#include "core/Common.h"
#include "Shared_generated.h"

namespace attrib {

using Types = network::messages::AttribType;

inline Types getType(const char* name) {
	const char **names = network::messages::EnumNamesAttribType();
	int i = 0;
	while (*names) {
		if (!strcmp(*names, name)) {
			return static_cast<Types>(i);
		}
		++i;
		++names;
	}
	return Types::NONE;
}

}

namespace std {
template<> struct hash<attrib::Types> {
	inline size_t operator()(const attrib::Types &s) const {
		return static_cast<size_t>(s);
	}
};
}
