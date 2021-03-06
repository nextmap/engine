/**
 * @file
 */

#pragma once

#include "ResponseParser.h"
#include "core/Common.h"
#include "core/String.h"

namespace http {

class HttpClient {
private:
	core::String _baseUrl;
public:
	HttpClient(const core::String &baseUrl = "");

	/**
	 * @brief Change the base url for the http client that is put in front of every request
	 * @return @c false if the given base url is not a valid url
	 */
	bool setBaseUrl(const core::String &baseUrl);

	ResponseParser get(CORE_FORMAT_STRING const char *msg, ...) CORE_PRINTF_VARARG_FUNC(2);
};

}