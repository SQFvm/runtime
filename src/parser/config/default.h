#pragma once
#include "../../runtime/parser/config.h"
#include "../../runtime/logging.h"
#include "../../runtime/diagnostics/diag_info.h"
#include "../../runtime/confighost.h"
#include "../../runtime/fileio.h"
#include "../../runtime/util.h"

#include <string>
#include <string_view>
#include <vector>

namespace sqf::parser::config
{
	class impl_default: public ::sqf::runtime::parser::config, public CanLog
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
			::sqf::runtime::fileio::pathinfo path;
			nodetype kind;
			std::vector<astnode> children;

			astnode() : offset(0), length(0), line(0), col(0), kind(nodetype::NA) {}
		};
	private:
		class instance
		{
		private:
			impl_default& owner;
#ifdef DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE
			size_t ___dbg_line_count;
#endif // DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE
		public:
			instance(
				impl_default& owner,
				std::string& contents,
				::sqf::runtime::fileio::pathinfo file
				) : owner(owner),
				m_contents(contents),
				m_file(file)
			{
				::sqf::runtime::diagnostics::diag_info dinf(1, 0, 0, m_file, {});
				m_info = dinf;
			}
			::sqf::runtime::diagnostics::diag_info m_info;
			::sqf::parser::util::string_ref m_contents;
			::sqf::runtime::fileio::pathinfo m_file;

			void skip();

			size_t endchr(size_t off);
			size_t identifier(size_t off);
			size_t operator_(size_t off);
			size_t hexadecimal(size_t off);
			size_t numsub(size_t off);
			size_t num(size_t off);
			size_t anytext(size_t off);
			bool NODELIST_start(size_t curoff);
			void NODELIST(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool NODE_start(size_t curoff);
			void NODE(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool CONFIGNODE_start(size_t curoff);
			void CONFIGNODE(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool DELETENODE_start(size_t curoff);
			void DELETENODE(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool VALUENODE_start(size_t curoff);
			void VALUENODE(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool STRING_start(size_t curoff);
			void STRING(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool NUMBER_start(size_t curoff);
			void NUMBER(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool LOCALIZATION_start(size_t curoff);
			void LOCALIZATION(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool ARRAY_start(size_t curoff);
			void ARRAY(::sqf::parser::config::impl_default::astnode& root, bool& errflag);
			bool VALUE_start(size_t curoff);
			void VALUE(::sqf::parser::config::impl_default::astnode& root, bool& errflag);

			::sqf::parser::config::impl_default::astnode parse(bool& errflag);
		};
		bool apply_to_confighost(::sqf::parser::config::impl_default::astnode& node, ::sqf::runtime::confighost& confighost, ::sqf::runtime::confignav parent);
	public:
		impl_default(Logger& logger) : CanLog(logger) { }
		virtual ~impl_default() override { };

		virtual bool check_syntax(std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) override
		{
			instance i(*this, contents, pathinfo);
			bool errflag = false;
			auto root = i.parse(errflag);
			return !errflag;
		}
		virtual bool parse(::sqf::runtime::confighost& target, std::string contents, ::sqf::runtime::fileio::pathinfo pathinfo) override
		{
			instance i(*this, contents, pathinfo);
			bool errflag = false;
			auto root = i.parse(errflag);
			if (errflag)
			{
				return {};
			}
			auto conf_root = target.root();
			return apply_to_confighost(root, target, conf_root);
		}
	};
}
