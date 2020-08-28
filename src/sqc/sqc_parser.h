#pragma once
#include "../runtime/parser/sqf.h"
#include "../runtime/logging.h"
#include "../runtime/diagnostics/diag_info.h"
#include "../runtime/fileio.h"
#include "../runtime/util.h"

#include <string>
#include <string_view>
#include <vector>


namespace sqf::sqc::bison
{
	struct astnode;
}
namespace sqf::sqc
{
	class parser : public ::sqf::runtime::parser::sqf, public CanLog
	{
	private:
		constexpr static const char* __scopename_function = "___sqc_func";
		void to_assembly(::sqf::runtime::runtime& runtime, std::vector<::sqf::runtime::instruction::sptr>& set, std::vector<std::string> locals, ::sqf::sqc::bison::astnode& current_node);
	public:
		parser(Logger& logger) : CanLog(logger)
		{
		}
		virtual ~parser() override { };
		virtual bool check_syntax(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;
		virtual std::optional<::sqf::runtime::instruction_set> parse(::sqf::runtime::runtime& runtime, std::string contents, ::sqf::runtime::fileio::pathinfo file) override;
	};
}
