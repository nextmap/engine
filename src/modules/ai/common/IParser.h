/**
 * @file
 */
#pragma once

#include "core/Common.h"
#include "StringUtil.h"
#include <SDL_stdinc.h>
#include <stdarg.h>

namespace ai {

class IParser {
private:
	core::String _error;

protected:
	void setError(CORE_FORMAT_STRING const char* msg, ...) CORE_PRINTF_VARARG_FUNC(2);

	inline void resetError() {
		_error = "";
	}

	inline core::String getBetween (const core::String& str, const core::String& tokenStart, const core::String& tokenEnd) {
		const std::size_t start = str.find(tokenStart);
		if (start == core::String::npos) {
			return "";
		}

		const std::size_t end = str.find(tokenEnd);
		if (end == core::String::npos) {
			setError("syntax error - expected %s", tokenEnd.c_str());
			return "";
		}
		const size_t startIndex = start + 1;
		const size_t endIndex = end - startIndex;
		if (endIndex <= 0) {
			return "";
		}
		const core::String& between = str.substr(startIndex, endIndex);
		return between;
	}

public:
	const core::String& getError() const;
};

inline void IParser::setError(const char* msg, ...) {
	va_list args;
	va_start(args, msg);
	char buf[1024];
	SDL_vsnprintf(buf, sizeof(buf), msg, args);
	va_end(args);
	_error = buf;
}


inline const core::String& IParser::getError() const {
	return _error;
}

}
