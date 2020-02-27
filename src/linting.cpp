#include "linting.h"
#include "virtualmachine.h"
#include "parsing/parsesqf.h"
#include "logging.h"
#include <algorithm>
#include <locale>
#include <cctype>

namespace err = logmessage::linting;
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
			std::vector<std::string> m_operators;

			void check(sqf::virtualmachine* vm, const char* code, const sqf::parse::astnode& node, sqf::virtualmachine::evaction act)
			{
				auto kind = static_cast<sqf::parse::asttype::sqf>(node.kind);
				switch (kind)
				{
				case sqf::parse::asttype::sqf::UNARYOP:
					if (act == sqf::virtualmachine::evaction::enter)
					{
						std::string operatorname = node.content;
						std::transform(operatorname.begin(), operatorname.end(), operatorname.begin(), [](unsigned char c) { return std::tolower(c); });
						m_operators.push_back(operatorname);
					}
					break;
				case sqf::parse::asttype::sqf::UNARYEXPRESSION:
					if (act == sqf::virtualmachine::evaction::exit)
					{
						m_operators.pop_back();
					}
					break;
				case sqf::parse::asttype::sqf::BINARYOP:
					if (act == sqf::virtualmachine::evaction::enter)
					{
						std::string operatorname = node.content;
						std::transform(operatorname.begin(), operatorname.end(), operatorname.begin(), [](unsigned char c) { return std::tolower(c); });
						m_operators.push_back(operatorname);
					}
					break;
				case sqf::parse::asttype::sqf::BINARYEXPRESSION:
					if (act == sqf::virtualmachine::evaction::exit)
					{
						m_operators.pop_back();
					}
					break;
				case sqf::parse::asttype::sqf::STRING:
					if (act == sqf::virtualmachine::evaction::enter)
					{
						auto operatorname = m_operators.back();
						if (operatorname == "private" ||
							operatorname == "for" ||
							(operatorname == "params" && kind == sqf::parse::asttype::sqf::STRING))
						{
							auto varname = node.content;
							std::transform(varname.begin(), varname.end(), varname.begin(), [](unsigned char c) { return std::tolower(c); });
							m_existing_variables.push_back(varname);
							m_existing_variables2.back().push_back(varname);
						}
					}
					break;
				case sqf::parse::asttype::sqf::SQF:
					if (act == sqf::virtualmachine::evaction::enter)
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
					else if (act == sqf::virtualmachine::evaction::exit)
					{
						m_existing_variables.clear();
						m_existing_variables2.clear();
					}
					break;
				case sqf::parse::asttype::sqf::CODE:
				{
					if (act == sqf::virtualmachine::evaction::enter)
					{
						m_existing_variables2.push_back({});
					}
					else if (act == sqf::virtualmachine::evaction::exit)
					{
						for (size_t i = 0; i < m_existing_variables2.back().size(); i++)
						{
							m_existing_variables.pop_back();
						}
						m_existing_variables2.pop_back();
					}
				}
				break;
				case sqf::parse::asttype::sqf::ASSIGNMENT:
				case sqf::parse::asttype::sqf::ASSIGNMENTLOCAL:
				{
					if (act != sqf::virtualmachine::evaction::enter)
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
				case sqf::parse::asttype::sqf::VARIABLE:
				{
					if (act != sqf::virtualmachine::evaction::enter)
					{
						break;
					}
					auto varname = node.content;
					if (varname.length() < 1 || varname[0] != '_')
					{
						break;
					}
					std::transform(varname.begin(), varname.end(), varname.begin(), [](unsigned char c) { return std::tolower(c); });

					auto find_result = std::find(m_existing_variables.begin(), m_existing_variables.end(), varname);
					if (find_result == m_existing_variables.end())
					{
						vm->logmsg(err::UnassignedVariable(node, varname));
					}
				}
				break;
				default:
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
					sqf::virtualmachine * vm, const char* code, const sqf::parse::astnode& node, sqf::virtualmachine::evaction act) mutable -> void
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