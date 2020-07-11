#include "make_array.h"
#include "../runtime/value.h"
#include "../runtime/data.h"
#include "../runtime/runtime.h"
#include "../runtime/logging.h"
#include "../types/d_array.h"

void sqf::opcodes::make_array::execute(sqf::runtime::runtime& vm) const
{
	auto vec = std::vector<sqf::runtime::value>(m_array_size);
	for (size_t i = m_array_size - 1; i != (size_t)~0; i--)
	{
		bool flag;
		auto opt = vm.active_context()->pop_value();
		if (!opt.has_value())
		{
			vm.__logmsg(logmessage::runtime::StackCorruptionMissingValues(diag_info(), m_array_size, m_array_size - i));
			break;
		}
		else
		{
			vec[i] = *opt;
		}
	}
	vm.active_context()->push_value(std::make_shared<sqf::types::d_array>(vec));
}