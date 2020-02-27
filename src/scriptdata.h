#pragma once
#include <string>
#include <memory>
#include <vector>

#include "data.h"
namespace sqf
{
	class vmstack;
	class scriptdata : public data, public std::enable_shared_from_this<scriptdata>
	{
	private:
		std::shared_ptr<sqf::vmstack> m_vmstack;
		size_t m_script_id;
	public:
		scriptdata();
		std::shared_ptr<sqf::vmstack> vmstack() const { return m_vmstack; }
		bool is_done() const;
		std::string tosqf() const override;
		bool equals(std::shared_ptr<data> d) const override { return this == d.get(); }
        sqf::type dtype() const override { return sqf::type::SCRIPT; }
		size_t script_id() { return m_script_id; }
	};
}