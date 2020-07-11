#pragma once
#include "../fileio.h"
#include "../runtime.h"

namespace sqf
{
	namespace runtime
	{
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
				virtual std::string preprocess(::sqf::runtime::runtime& runtime, ::sqf::runtime::fileio::pathinfo pathinfo) override { return runtime.fileio().read_file(pathinfo); }
		};
	}
}