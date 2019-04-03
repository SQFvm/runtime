#pragma once
#include <memory>
#include "instruction.h"

namespace sqf
{
	class nularcmd;
	namespace inst
	{
		class callnular : public instruction
		{
		private:
			std::shared_ptr<sqf::nularcmd> mcmd;
		public:
			callnular(std::shared_ptr<sqf::nularcmd> cmd) { mcmd = cmd; }
			void execute(virtualmachine*) const override;
			insttype thistype() const override { return insttype::callnular; }
			std::string to_string() const override;
		};
	}
}