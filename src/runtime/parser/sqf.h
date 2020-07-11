#pragma once
#include "../fileio.h"
#include "../instruction_set.h"

#include <optional>

namespace sqf
{
	namespace runtime
	{
		class runtime;
		namespace parser
		{
			class sqf
			{
			public:
				virtual ~sqf() = 0;
				virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents) = 0;
				virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents) = 0;
			};
		}
	}
	namespace parser::sqf
	{
		class disabled : public ::sqf::runtime::parser::sqf
		{
		public:
			virtual ~disabled() override { return; };
			virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents) override { return false; }
			virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents) override { return {}; }
		};
	}
}