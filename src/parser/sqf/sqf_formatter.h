#pragma once
#include "tokenizer.hpp"
#include "parser.tab.hh"
#include "../../runtime/fileio.h"
#include "../../runtime/runtime.h"
#include "sqf_parser.hpp"

#include <string>
#include <vector>

namespace sqf::parser::sqf {
	class formatter {
	private:
		bool hasError = false;
		::sqf::runtime::runtime& runtime;
		std::string contents;
		::sqf::runtime::fileio::pathinfo file;
		::sqf::parser::sqf::bison::astnode res;
	public:
		formatter(::sqf::runtime::runtime& r, std::string c, ::sqf::runtime::fileio::pathinfo f);
		::sqf::parser::sqf::bison::astnode& getRes() { return this->res; }
		void prettify(const ::sqf::parser::sqf::bison::astnode& node, size_t depth, std::ostringstream& buff);
	};
}
