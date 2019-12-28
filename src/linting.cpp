#include "linting.h"
#include "virtualmachine.h"
#include "parsesqf.h"
#include <algorithm>

namespace sqf
{
	namespace linting
	{

		class private_var_usage_check
		{
		public:
			/// List of Existing Variables
			std::vector<std::string> m_existing_variables;
			/// List of Existing Variables sorted by introduced code node.
			std::vector<std::vector<std::string>> m_existing_variables2;

			void check(sqf::virtualmachine* vm, const char* code, const astnode& node, sqf::virtualmachine::action act)
			{
				switch (node.kind)
				{
				case sqf::parse::sqf::sqfasttypes::SQF:
					if (act == sqf::virtualmachine::action::enter)
					{
						m_existing_variables2.push_back({});
						m_existing_variables.push_back("_this");
						m_existing_variables.push_back("_x");
						m_existing_variables.push_back("_exception");
						m_existing_variables.push_back("_foreachindex");
						m_existing_variables.push_back("_thiseventhandler");
						m_existing_variables.push_back("_thisfsm");
						m_existing_variables.push_back("_thisscript");
					}
					else if (act == sqf::virtualmachine::action::exit)
					{
						m_existing_variables.clear();
						m_existing_variables2.clear();
					}
					break;
				case sqf::parse::sqf::sqfasttypes::CODE:
				{
					if (act == sqf::virtualmachine::action::enter)
					{
						m_existing_variables2.push_back({});
					}
					else if (act == sqf::virtualmachine::action::exit)
					{
						for (size_t i = 0; i < m_existing_variables2.back().size(); i++)
						{
							m_existing_variables.pop_back();
						}
						m_existing_variables2.pop_back();
					}
				}
				break;
				case sqf::parse::sqf::sqfasttypes::ASSIGNMENT:
				case sqf::parse::sqf::sqfasttypes::ASSIGNMENTLOCAL:
				{
					if (act != sqf::virtualmachine::action::enter)
					{
						break;
					}
					auto varname = node.children[0].content;
					if (varname.length() < 1 || varname[0] != '_')
					{
						break;
					}
					std::transform(varname.begin(), varname.end(), varname.begin(), [](unsigned char c) { return std::tolower(c); });

					m_existing_variables.push_back(varname);
					m_existing_variables2.back().push_back(varname);
				}
				break;
				case sqf::parse::sqf::sqfasttypes::VARIABLE:
				{
					if (act != sqf::virtualmachine::action::enter)
					{
						break;
					}
					auto varname = node.content;
					std::transform(varname.begin(), varname.end(), varname.begin(), [](unsigned char c) { return std::tolower(c); });

					auto find_result = std::find(m_existing_variables.begin(), m_existing_variables.end(), varname);
					if (find_result == m_existing_variables.end())
					{
						vm->wrn() << vm->dbgsegment(code, node.offset, node.length) << "[WRN][L" << node.line << "|C" << node.col << "]\t" << "Unassigned Variable Detected." << std::endl;
					}
				}
				break;
				}
			}
		};

		void add_to(virtualmachine* vm, check type)
		{
			switch (type)
			{
			case sqf::linting::private_var_usage:
			{
				vm->register_callback([instance = private_var_usage_check()](
					sqf::virtualmachine * vm, const char* code, const astnode & node, sqf::virtualmachine::action act) mutable -> void
				{
					instance.check(vm, code, node, act);
				});
			}
			break;
			default:
				throw std::runtime_error("Missing implementation for type");
				break;
			}
		}
	}
}