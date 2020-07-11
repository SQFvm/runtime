#pragma once
#include "../fileio.h"

namespace sqf
{
	namespace runtime
	{
		class runtime;
		namespace parser
		{
			class preprocessor
			{
			public:
				virtual ~preprocessor() = 0;
				virtual std::string preprocess(::sqf::runtime::runtime& runtime, ::sqf::runtime::fileio::pathinfo pathinfo) = 0;
			};
		}
	}
	namespace parser::preprocessor
	{
		class passthrough : public ::sqf::runtime::parser::preprocessor
		{
			public:
				virtual ~passthrough() override { return; };
				virtual std::string preprocess(::sqf::runtime::runtime & runtime, ::sqf::runtime::fileio::pathinfo pathinfo) override;
		};
	}
}