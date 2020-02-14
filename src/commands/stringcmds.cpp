#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include <algorithm>
#include <sstream>
#include <cmath>

namespace err = logmessage::runtime;
using namespace sqf;
namespace
{
	value count_string(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_string();
		return r.length();
	}
	value toupper_string(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_string();
		std::transform(r.begin(), r.end(), r.begin(), ::toupper);
		return r;
	}
	value tolower_string(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_string();
		std::transform(r.begin(), r.end(), r.begin(), ::tolower);
		return r;
	}
	value select_string_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto str = left.as_string();
		auto arr = right.as_vector();
		if (arr.empty())
		{
			vm->logmsg(err::ExpectedArrayToHaveElements(*vm->current_instruction()));
			return {};
		}
		if (arr[0].dtype() != type::SCALAR)
		{
			vm->logmsg(err::ExpectedArrayTypeMissmatch(*vm->current_instruction(), 0, sqf::type::SCALAR, arr[0].dtype()));
			return {};
		}
		int start = static_cast<int>(std::round(arr[0].as_float()));
		if (start < 0)
		{
			vm->logmsg(err::NegativeIndexWeak(*vm->current_instruction()));
			vm->logmsg(err::ReturningEmptyString(*vm->current_instruction()));
			return "";
		}
		if (start >= static_cast<int>(str.length()))
		{
			vm->logmsg(err::IndexOutOfRangeWeak(*vm->current_instruction(), str.length(), start));
			vm->logmsg(err::ReturningEmptyString(*vm->current_instruction()));
			return "";
		}
		if (arr.size() >= 2)
		{
			if (arr[1].dtype() != type::SCALAR)
			{
				vm->logmsg(err::ExpectedArrayTypeMissmatch(*vm->current_instruction(), 1, sqf::type::SCALAR, arr[1].dtype()));
				return {};
			}
			int length = static_cast<int>(std::round(arr[1].as_float()));
			if (length < 0)
			{
				vm->logmsg(err::NegativeIndexWeak(*vm->current_instruction()));
				vm->logmsg(err::ReturningEmptyString(*vm->current_instruction()));
				return "";
			}
			return str.substr(start, length);
		}
		return str.substr(start);
	}
	value format_array(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_vector();
		if (r.empty())
		{
			vm->logmsg(err::ExpectedArrayToHaveElementsWeak(*vm->current_instruction()));
			vm->logmsg(err::ReturningEmptyString(*vm->current_instruction()));
			return "";
		}
		if (r[0].dtype() != type::STRING)
		{
			vm->logmsg(err::ExpectedArrayTypeMissmatchWeak(*vm->current_instruction(), 0, sqf::type::STRING, r[0].dtype()));
			vm->logmsg(err::ReturningEmptyString(*vm->current_instruction()));
			return "";
		}
		auto format = r[0].as_string();
		std::stringstream sstream;
		size_t off = 0;
		size_t newoff;
		while ((newoff = format.find(L'%', off)) != std::string::npos)
		{
			sstream << format.substr(off, newoff - off);
			newoff++;

			if (!(format[newoff] >= '0' && format[newoff] <= '9'))
			{
				vm->logmsg(err::FormatInvalidPlaceholder(*vm->current_instruction(), format[newoff], newoff));
				newoff++;
			}
			else
			{
				auto num = std::stoi(format.substr(newoff));
				if (num >= static_cast<int>(r.size()))
				{
					vm->logmsg(err::IndexOutOfRangeWeak(*vm->current_instruction(), r.size(), num));
				}
				else if (r[num].dtype() == STRING)
				{
					sstream << r[num].as_string();
				}
				else
				{
					sstream << r[num].tosqf();
				}
				while (format[newoff] >= '0' && format[newoff] <= '9') newoff++;
			}
			off = newoff;
		}
		sstream << (format.size() >= off ? format.substr(off) : "");
		return sstream.str();
	}
	value toarray_string(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_string();
		auto arr = std::vector<value>(r.size());
		for (size_t i = 0; i < r.size(); i++)
		{
			arr[i] = static_cast<int>(r[i]);
		}
		return value(arr);
	}
	value tostring_array(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_vector();
		std::stringstream sstream;
		for (size_t i = 0; i < r.size(); i++)
		{
			auto& val = r[i];
			if (val.dtype() == SCALAR)
			{
				sstream << static_cast<char>(val.as_int());
			}
			else
			{
				vm->logmsg(err::ExpectedArrayTypeMissmatch(*vm->current_instruction(), i, sqf::type::SCALAR, val.dtype()));
			}
		}
		return sstream.str();
	}
	value joinstring_array_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_vector();
		auto r = right.as_string();
		std::stringstream sstream;
		bool separator = false;
		for (auto& it : l)
		{
			if (separator)
			{
				if (it.dtype() == sqf::type::STRING)
				{
					sstream << r << it.as_string();
				}
				else
				{
					sstream << r << it.tosqf();
				}
			}
			else
			{
				separator = true;
				if (it.dtype() == sqf::type::STRING)
				{
					sstream << it.as_string();
				}
				else
				{
					sstream << it.tosqf();
				}
			}
		}
		return sstream.str();
	}
	value plus_string_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_string();
		auto r = right.as_string();
		std::stringstream sstream;
		sstream << l << r;
		return sstream.str();
	}
	value find_string_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_string();
		auto r = right.as_string();
		size_t res = l.find(r);
        auto x = res == std::string::npos;
		return res == std::string::npos ? -1.f : res;
	}
	value splitstring_string_string(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_string();
		auto r = right.as_string();
		std::vector<value> values;
		if (r.empty())
		{
			for (auto c : l)
			{
				values.push_back(c);
			}
			return values;
		}
		// Avoid using actual strtok method due to concerns regarding the way
		// it works (Reentrancy variance)
		size_t match_length = 0;
		for (size_t i = 0; i < l.length(); i++)
		{
			char lc = l[i];
			bool hit = false;
			for (auto rc : r)
			{
				if (lc == rc)
				{
					if (match_length > 0)
					{
						values.push_back(l.substr(i - match_length, match_length));
						match_length = 0;
					}
					hit = true;
					break;
				}
			}
			if (!hit)
			{
				match_length++;
			}
		}
		if (match_length != 0)
		{
			values.push_back(l.substr(l.size() - match_length));
		}
		return values;
	}
}
void sqf::commandmap::initstringcmds()
{
	add(unary("count", sqf::type::STRING, "Can be used to count: the number of characters in a string.", count_string));
	add(unary("toLower", sqf::type::STRING, "Converts the supplied string to all lowercase characters.", tolower_string));
	add(unary("toUpper", sqf::type::STRING, "Converts the supplied string to all uppercase characters.", toupper_string));
	add(binary(4, "select", type::STRING, type::ARRAY, "Selects a range of characters in provided string, starting at element 0 index, ending at either end of the string or the provided element 1 length.", select_string_array));
	add(unary("format", sqf::type::ARRAY, "Composes a string containing other variables or other variable types. Converts any variable type to a string.", format_array));
	add(unary("toArray", sqf::type::STRING, "Converts the supplied String into an Array of Numbers.", toarray_string));
	add(unary("toString", sqf::type::ARRAY, "Converts the supplied String into an Array of Numbers.", tostring_array));
	add(binary(4, "joinString", sqf::type::ARRAY, sqf::type::STRING, "Joins array into String with provided separator. Array can be of mixed types, all elements will be converted to String prior to joining, but the fastest operation is on the array of Strings.", joinstring_array_string));
	add(binary(6, "+", sqf::type::STRING, sqf::type::STRING, "Concatinates two strings together.", plus_string_string));
	add(binary(4, "find", sqf::type::STRING, sqf::type::STRING, "Searches for a string within a string. Returns the 0 based index on success or -1 if not found.", find_string_string));
	add(binary(4, "splitString", type::STRING, type::STRING, "An SQF version of C++ strtok. "
		"Splits given string str into an array of tokens according to given delimiters. "
		"In addition, if empty string "" is used for delimiters, str is split by each character.", splitstring_string_string));
}
