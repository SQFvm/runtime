#pragma once
#include "../../runtime/parser/config.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"
#include "../../runtime/confighost.h"

#include <string>
#include <string_view>
#include <vector>

namespace sqf::parser::config
{
	class default: public ::sqf::runtime::parser::config, public CanLog
	{
	public:
		enum class nodetype
		{
			NA = 0,
			NODELIST,
			NODE,
			CONFIGNODE,
			CONFIGNODE_PARENTIDENT,
			VALUENODE,
			VALUENODE_ADDARRAY,
			DELETENODE,
			STRING,
			NUMBER,
			HEXNUMBER,
			LOCALIZATION,
			ARRAY,
			VALUE
		};
		struct astnode
		{
			size_t offset;
			size_t length;
			size_t line;
			size_t col;
			std::string content;
			std::string file;
			nodetype kind;
			std::vector<astnode> children;

			astnode() : offset(0), length(0), line(0), col(0), kind(nodetype::NA) {}
		};
	private:
		::sqf::runtime::diagnostics::diag_info m_info;
		std::string_view m_contents;
		std::string m_file;

		void skip();

		size_t endchr(size_t off);
		size_t identifier(size_t off);
		size_t operator_(size_t off);
		size_t hexadecimal(size_t off);
		size_t numsub(size_t off);
		size_t num(size_t off);
		size_t anytext(size_t off);
		bool NODELIST_start(size_t curoff);
		void NODELIST(astnode& root, bool& errflag);
		bool NODE_start(size_t curoff);
		void NODE(astnode& root, bool& errflag);
		bool CONFIGNODE_start(size_t curoff);
		void CONFIGNODE(astnode& root, bool& errflag);
		bool DELETENODE_start(size_t curoff);
		void DELETENODE(astnode& root, bool& errflag);
		bool VALUENODE_start(size_t curoff);
		void VALUENODE(astnode& root, bool& errflag);
		bool STRING_start(size_t curoff);
		void STRING(astnode& root, bool& errflag);
		bool NUMBER_start(size_t curoff);
		void NUMBER(astnode& root, bool& errflag);
		bool LOCALIZATION_start(size_t curoff);
		void LOCALIZATION(astnode& root, bool& errflag);
		bool ARRAY_start(size_t curoff);
		void ARRAY(astnode& root, bool& errflag);
		bool VALUE_start(size_t curoff);
		void VALUE(astnode& root, bool& errflag);

		astnode parse(bool& errflag);
		bool sqf::parser::config::default::apply_to_confighost(sqf::parser::config::default::astnode& node, ::sqf::runtime::confighost& confighost, ::sqf::runtime::confighost::config& parent);

	public:
		default(
			Logger& logger,
			std::string_view contents,
			std::string_view file
		) : CanLog(logger),
			m_contents(contents),
			m_file(file)
		{
			::sqf::runtime::diagnostics::diag_info dinf(1, 0, 0, m_file, "");
			m_info = dinf;
		}
		virtual ~default() override { };

		virtual bool check_syntax(std::string contents) override
		{
			bool success = false;
			parse(success);
			return success;
		}
		virtual bool parse(::sqf::runtime::confighost& target, std::string contents) override
		{
			bool success = false;
			auto root = parse(success);
			if (!success)
			{
				return {};
			}
			return apply_to_confighost(root, target, target.root());
		}
	};
}