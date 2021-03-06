/**
 * @file
 * @ingroup Condition
 */

#include "ConditionParser.h"

namespace ai {

ConditionParser::ConditionParser(const IAIFactory& aiFactory, const core::String& conditionString) :
		IParser(), _aiFactory(aiFactory) {
	_conditionString = ai::Str::eraseAllSpaces(conditionString);
}

ConditionParser::~ConditionParser() {
}

void ConditionParser::splitConditions(const core::String& string, std::vector<core::String>& tokens) const {
	int inParameter = 0;
	int inChildren = 0;
	core::String token;
	for (auto i = string.begin(); i != string.end(); ++i) {
		if (*i == '{') {
			++inParameter;
		} else if (*i == '}') {
			--inParameter;
		} else if (*i == '(') {
			++inChildren;
		} else if (*i == ')') {
			--inChildren;
		}

		if (inParameter == 0 && inChildren == 0) {
			if (*i == ',') {
				tokens.push_back(token);
				token.clear();
				continue;
			}
		}
		token += *i;
	}
	tokens.push_back(token);
}

bool ConditionParser::fillInnerFilters(FilterFactoryContext& ctx, const core::String& filterStr) {
	std::vector<core::String> conditions;
	splitConditions(filterStr, conditions);
	if (conditions.size() > 1) {
		for (auto i = conditions.begin(); i != conditions.end(); ++i) {
			if (!fillInnerFilters(ctx, *i)) {
				return false;
			}
		}
		return true;
	}

	core::String parameters;
	std::size_t n = filterStr.find("(");
	if (n == core::String::npos || filterStr.find("{") < n) {
		parameters = getBetween(filterStr, "{", "}");
		n = filterStr.find("{");
	}

	core::String name;
	if (n != core::String::npos) {
		name = filterStr.substr(0, n);
	} else {
		name = filterStr;
	}
	FilterFactoryContext ctxInner(parameters);
	n = filterStr.find("(");
	if (n != core::String::npos) {
		const std::size_t r = filterStr.rfind(")");
		if (r == core::String::npos) {
			setError("syntax error, missing closing brace");
			return false;
		}
		const core::String& inner = filterStr.substr(n + 1, r - n - 1);
		if (!fillInnerFilters(ctxInner, inner)) {
			return false;
		}
	}
	const FilterPtr& c = _aiFactory.createFilter(name, ctxInner);
	if (!c) {
		setError("could not create filter for %s", name.c_str());
		return false;
	}
	ctx.filters.push_back(c);
	return true;
}

bool ConditionParser::fillInnerConditions(ConditionFactoryContext& ctx, const core::String& conditionStr) {
	std::vector<core::String> conditions;
	splitConditions(conditionStr, conditions);
	if (conditions.size() > 1) {
		for (auto i = conditions.begin(); i != conditions.end(); ++i) {
			if (!fillInnerConditions(ctx, *i)) {
				return false;
			}
		}
	} else {
		core::String parameters;
		std::size_t n = conditionStr.find("(");
		if (n == core::String::npos || conditionStr.find("{") < n) {
			parameters = getBetween(conditionStr, "{", "}");
			n = conditionStr.find("{");
		}

		core::String name;
		if (n != core::String::npos) {
			name = conditionStr.substr(0, n);
		} else {
			name = conditionStr;
		}
		// filter condition is a little bit special and deserves some extra attention
		if (ctx.filter) {
			FilterFactoryContext ctxInner(parameters);
			n = conditionStr.find("(");
			if (n != core::String::npos) {
				const std::size_t r = conditionStr.rfind(")");
				if (r == core::String::npos) {
					setError("syntax error, missing closing brace");
					return false;
				}
				const core::String& inner = conditionStr.substr(n + 1, r - n - 1);
				if (!fillInnerFilters(ctxInner, inner)) {
					return false;
				}
			}
			const FilterPtr& c = _aiFactory.createFilter(name, ctxInner);
			if (!c) {
				setError("could not create filter for %s", name.c_str());
				return false;
			}
			ctx.filters.push_back(c);
		} else {
			ConditionFactoryContext ctxInner(parameters);
			ctxInner.filter = name == FILTER_NAME;
			n = conditionStr.find("(");
			if (n != core::String::npos) {
				const std::size_t r = conditionStr.rfind(")");
				if (r == core::String::npos) {
					setError("syntax error, missing closing brace");
					return false;
				}
				const core::String& inner = conditionStr.substr(n + 1, r - n - 1);
				if (!fillInnerConditions(ctxInner, inner)) {
					return false;
				}
			}
			const ConditionPtr& c = _aiFactory.createCondition(name, ctxInner);
			if (!c) {
				setError("could not create inner condition for %s", name.c_str());
				return false;
			}
			ctx.conditions.push_back(c);
		}
	}
	return true;
}

ConditionPtr ConditionParser::getCondition() {
	resetError();
	core::String parameters;
	std::size_t n = _conditionString.find("(");
	if (n == core::String::npos || _conditionString.find("{") < n) {
		parameters = getBetween(_conditionString, "{", "}");
		n = _conditionString.find("{");
	}
	core::String name;
	if (n != core::String::npos) {
		name = _conditionString.substr(0, n);
	} else {
		name = _conditionString;
	}
	ConditionFactoryContext ctx(parameters);
	ctx.filter = name == FILTER_NAME;
	n = _conditionString.find("(");
	if (n != core::String::npos) {
		const std::size_t r = _conditionString.rfind(")");
		if (r == core::String::npos) {
			setError("syntax error, missing closing brace");
			return ConditionPtr();
		}
		const core::String& inner = _conditionString.substr(n + 1, r - n - 1);
		if (!fillInnerConditions(ctx, inner)) {
			return ConditionPtr();
		}
	} else if (ctx.filter) {
		setError("missing details for Filter condition");
		return ConditionPtr();
	}
	const ConditionPtr& c = _aiFactory.createCondition(name, ctx);
	if (!c) {
		setError("could not create condition for %s", name.c_str());
		return ConditionPtr();
	}
	return c;
}

}
