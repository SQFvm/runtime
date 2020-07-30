#include "ops_config.h"
#include "../runtime/value.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"

#include "../runtime/d_string.h"
#include "../runtime/d_array.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "d_config.h"


namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;
using namespace std::string_literals;

#include <algorithm>

namespace
{
	value greaterthengreaterthen_config_string(runtime& runtime, value::cref left, value::cref right)
	{
		auto cd = left.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningConfigNull((*runtime.context_active().current_frame().current())->diag_info()));
			return confighost::config();
		}
		auto target = right.data<d_string, std::string>();
		auto opt = cd.navigate(runtime.confighost(), target);

		if (opt.has_value())
		{
			return opt.value();
		}
		else
		{
			// Get navigation path
			std::vector<std::string> path;
			do
			{
				path.push_back(std::string(cd.name()));
				cd = cd.parent_logical(runtime.confighost());
			} while (cd.has_parent_logical());

			std::reverse(path.begin(), path.end());
			runtime.__logmsg(err::ConfigEntryNotFoundWeak((*runtime.context_active().current_frame().current())->diag_info(), path, target));
			runtime.__logmsg(err::ReturningConfigNull((*runtime.context_active().current_frame().current())->diag_info()));
			return confighost::config();
		}
	}
	value confignull__(runtime& runtime)
	{
		return confighost::config();
	}
	value configfile__(runtime& runtime)
	{
		return runtime.confighost().root();
	}
	value configname_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.context_active().current_frame().current())->diag_info()));
			return ""s;
		}
		return cd.name();
	}
	value select_config_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto cd = left.data<d_config, confighost::config>();
		auto index = right.data<d_scalar, int>();
		if (index >= static_cast<int>(cd.children_size()) || index < 0)
		{
			runtime.__logmsg(err::IndexOutOfRangeWeak((*runtime.context_active().current_frame().current())->diag_info(), cd.children_size(), index));
			runtime.__logmsg(err::ReturningConfigNull((*runtime.context_active().current_frame().current())->diag_info()));
			return confighost::config();
		}
		return cd.at(runtime.confighost(), index);
	}
	value count_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningScalarZero((*runtime.context_active().current_frame().current())->diag_info()));
			return 0;
		}
		return cd.children_size();
	}
	value confighierarchy_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningScalarZero((*runtime.context_active().current_frame().current())->diag_info()));
			return 0;
		}
		std::vector<value> branch;
		do
		{
			branch.emplace_back(cd);
			cd = cd.parent_logical(runtime.confighost());
		} while (cd.has_parent_logical());

		std::reverse(branch.begin(), branch.end());
		return branch;
	}
	value inheritsfrom_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		return cd.parent_logical(runtime.confighost());
	}
	value isnumber_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		return cd.value().is<t_scalar>();
	}
	value istext_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		return cd.value().is<t_string>();
	}
	value isclass_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		return cd.children_size() > 0;
	}
	value isarray_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		return cd.value().is<t_array>();
	}
	value getnumber_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		return cd.value().data_try<d_scalar, float>(0.0F);
	}
	value gettext_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		return cd.value().data_try<d_string, std::string>(""s);
	}
	value getarray_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		if (cd.is_null())
		{
			runtime.__logmsg(err::ExpectedNonNullValue((*runtime.context_active().current_frame().current())->diag_info()));
			runtime.__logmsg(err::ReturningFalse((*runtime.context_active().current_frame().current())->diag_info()));
			return false;
		}
		auto data = cd.value().data_try<d_array>();
		if (data)
		{
			return data;
		}
		else
		{
			return std::make_shared<d_array>();
		}
	}
	value isnull_config(runtime& runtime, value::cref right)
	{
		auto cd = right.data<d_config, confighost::config>();
		return cd.is_null();
	}
	value configclasses_code_config(runtime& runtime, value::cref left, value::cref right)
	{
		class behavior_configclasses_exit : public frame::behavior
		{
		private:
			std::shared_ptr<d_array> m_out_arr;
			confighost::config::iterator_factory m_iterator_factory;
			confighost::config::iterator_factory::iterator m_iterator_current;
		public:
			behavior_configclasses_exit(confighost::config::iterator_factory fact) : m_out_arr(std::make_shared<d_array>()), m_iterator_factory(fact), m_iterator_current(fact.begin()) {}
			virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
			{
				auto res = runtime.context_active().pop_value();
				if (res.has_value())
				{
					auto value = res->data_try<d_boolean, bool>();
					if (value.has_value())
					{
						m_out_arr->push_back(m_iterator_current->get());
					}
					else
					{
						runtime.__logmsg(logmessage::runtime::TypeMissmatch((*frame.current())->diag_info(), t_boolean(), res->type()));
					}
				}
				else
				{
					runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue((*frame.current())->diag_info(), "configClasses"sv));
				}
				if (++m_iterator_current == m_iterator_factory.end())
				{
					runtime.context_active().push_value(m_out_arr);
					return result::ok;
				}
				else
				{
					frame["_x"] = m_iterator_current->get();
					return result::seek_start;
				}
			};
		};

		auto code = left.data<d_string, std::string>();
		auto conf = right.data<d_config, confighost::config>();

		if (conf.children_size() == 0)
		{
			return std::make_shared<d_array>();
		}
		else
		{
			auto& parser = runtime.parser_sqf();
			auto res = parser.parse(runtime, code, (*runtime.context_active().current_frame().current())->diag_info().path);

			if (res.has_value())
			{
				frame f(runtime.default_value_scope(), res.value(), std::make_shared<behavior_configclasses_exit>(conf.iterate(runtime.confighost())));
				f["_x"] = conf.at(runtime.confighost(), 0);
				runtime.context_active().push_frame(f);
			}
		}
		return {};
	}
	value configproperties_array(runtime& runtime, value::cref right)
	{
		class behavior_configproperties_exit : public frame::behavior
		{
		private:
			std::shared_ptr<d_array> m_out_arr;
			confighost::config::iterator_recursive_factory m_iterator_factory;
			confighost::config::iterator_recursive_factory::iterator m_iterator_current;
		public:
			behavior_configproperties_exit(confighost::config::iterator_recursive_factory fact) : m_out_arr(std::make_shared<d_array>()), m_iterator_factory(fact), m_iterator_current(fact.begin()) {}
			virtual result enact(sqf::runtime::runtime& runtime, sqf::runtime::frame& frame) override
			{
				auto res = runtime.context_active().pop_value();
				if (res.has_value())
				{
					auto value = res->data_try<d_boolean, bool>();
					if (value.has_value())
					{
						m_out_arr->push_back(m_iterator_current->get());
					}
					else
					{
						runtime.__logmsg(logmessage::runtime::TypeMissmatch((*frame.current())->diag_info(), t_boolean(), res->type()));
					}
				}
				else
				{
					runtime.__logmsg(logmessage::runtime::CallstackFoundNoValue((*frame.current())->diag_info(), "configClasses"sv));
				}
				if (++m_iterator_current == m_iterator_factory.end())
				{
					runtime.context_active().push_value(m_out_arr);
					return result::ok;
				}
				else
				{
					frame["_x"] = m_iterator_current->get();
					return result::seek_start;
				}
			};
		};

		auto arr = right.data<d_array>();
		if (!arr->check_type(runtime, std::array<sqf::runtime::type, 3>{ t_config(), t_string(), t_boolean() }, 1))
		{
			return {};
		}
		auto conf = arr->get<d_config, confighost::config>(0);
		auto code = arr->get<d_string, std::string>(1, "true"s);
		auto inherit = arr->get<d_boolean, bool>(2, true);
		if (inherit)
		{

			if (conf.children_size() == 0)
			{
				return std::make_shared<d_array>();
			}
			else
			{
				auto& parser = runtime.parser_sqf();
				auto res = parser.parse(runtime, code, (*runtime.context_active().current_frame().current())->diag_info().path);

				if (res.has_value())
				{
					frame f(runtime.default_value_scope(), res.value(), std::make_shared<behavior_configproperties_exit>(conf.iterate_recursive(runtime.confighost())));
					f["_x"] = conf.at(runtime.confighost(), 0);
					runtime.context_active().push_frame(f);
				}
			}
			return {};
		}
		else
		{
			return configclasses_code_config(runtime, code, conf);
		}
	}
}
void sqf::operators::ops_config(::sqf::runtime::runtime& runtime)
{
	using namespace ::sqf::runtime::sqfop;
	runtime.register_sqfop(nular("configNull", "A non-existing t_config().", confignull__));
	runtime.register_sqfop(nular("configFile", "Return root of t_config() entries hierarchy.", configfile__));
	runtime.register_sqfop(unary("configName", t_config(), "Returns name of t_config() entry.", configname_config));
	runtime.register_sqfop(binary(3, ">>", t_config(), t_string(), "Returns subentry of t_config() entry with given name.", greaterthengreaterthen_config_string));
	runtime.register_sqfop(binary(7, "/", t_config(), t_string(), "Returns subentry of t_config() entry with given name.", greaterthengreaterthen_config_string));
	runtime.register_sqfop(binary(4, "select", t_config(), t_scalar(), "Selects an element at given index from provided t_config().", select_config_scalar));
	runtime.register_sqfop(unary("count", t_config(), "Returns the number of sub-entries in provided t_config() object.", count_config));
	runtime.register_sqfop(unary("configHierarchy", t_config(), "Returns hierarchy of the given t_config() class.", confighierarchy_config));
	runtime.register_sqfop(unary("inheritsFrom", t_config(), "Returns base entry of t_config() entry.", inheritsfrom_config));
	runtime.register_sqfop(unary("isNumber", t_config(), "Returns base entry of t_config() entry.", isnumber_config));
	runtime.register_sqfop(unary("isText", t_config(), "Check if t_config() entry represents text.", istext_config));
	runtime.register_sqfop(unary("isClass", t_config(), "Check if t_config() entry represents t_config() class.", isclass_config));
	runtime.register_sqfop(unary("isArray", t_config(), "Check if t_config() entry represents array.", isarray_config));
	runtime.register_sqfop(unary("getNumber", t_config(), "Extract number from t_config() entry.", getnumber_config));
	runtime.register_sqfop(unary("getText", t_config(), "Extract text from t_config() entry.", gettext_config));
	runtime.register_sqfop(unary("getArray", t_config(), "Extract array from t_config() entry.", getarray_config));
	runtime.register_sqfop(unary("isNull", t_config(), "Checks whether the tested item is Null.", isnull_config));
	runtime.register_sqfop(binary(4, "configClasses", t_string(), t_config(), "Returns an array of t_config() entries which meet criteria in condition code. Command iterates through all available t_config() sub classes of the given t_config() class. Current looked at t_config() is stored in _x variable (similar to alternative count command implementation). Condition has to return true in order for the looked at t_config() to be added to the resulting array. Slightly faster than configProperties, but doesn't account for t_config() properties or inherited entries.", configclasses_code_config));
	runtime.register_sqfop(unary("configProperties", t_array(), "Returns an array of t_config() entries which meet criteria in condition code. Command iterates through available classes and t_config() properties for given t_config() entry. If 3rd param is true the search also includes inherited properties. Current looked at t_config() is stored in _x variable (similar to alternative count command implementation). Condition has to return true in order for the looked at property to be added to the resulting array. A bit slower than configClasses but allows to access inherited entires.", configproperties_array));
}