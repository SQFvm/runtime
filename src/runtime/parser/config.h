#pragma once
#include "../fileio.h"
#include "../instruction_set.h"

namespace sqf
{
	namespace runtime
	{
		class runtime;
		class confighost;
		namespace parser
		{
			class config
			{
			public:
				virtual ~config() = 0;
				virtual bool check_syntax(std::string contents) = 0;
				virtual bool parse(::sqf::runtime::confighost& target, std::string contents) = 0;
			};
		}
	}
	namespace parser::config
	{
		class disabled : public ::sqf::runtime::parser::config
		{
		public:
			virtual ~disabled() override { return; }
			virtual bool check_syntax(std::string contents) override { return false; }
			virtual bool parse(::sqf::runtime::confighost& target, std::string contents) override { return false; }
		};
	}
}