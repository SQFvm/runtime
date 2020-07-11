#pragma once
#include "../runtime/instruction.h"

namespace sqf::runtime
{
	class runtime;
}
namespace sqf::opcodes
{
	class make_array : public sqf::runtime::instruction
	{
	private:
		size_t m_array_size;

	public:
		make_array(size_t array_size) : m_array_size(array_size) {}
		virtual void execute(sqf::runtime::runtime& vm) const override;
		virtual std::string to_string() const override { return std::string("MAKEARRAY ") + std::to_string(m_array_size); }
		size_t array_size() const { return m_array_size; }
	};
}
