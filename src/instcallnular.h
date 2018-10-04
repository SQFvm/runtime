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
			virtual void execute(virtualmachine*) const;
			virtual insttype thistype() const { return insttype::callnular; }
		};
	}
}