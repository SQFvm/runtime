#pragma once
#include <string>
#include <memory>
#include "type.h"

namespace sqf
{
	class data
	{
	public:
		virtual std::string tosqf() const = 0;
		virtual bool equals(std::shared_ptr<data>) const = 0;
		virtual bool equals_exact(std::shared_ptr<data> other) const { return equals(other); }
        virtual type dtype() const = 0;
	};
}
