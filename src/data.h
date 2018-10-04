#pragma once
#include <string>
#include <memory>

namespace sqf
{
	class data
	{
	public:
		virtual std::string tosqf() const = 0;
		virtual bool equals(std::shared_ptr<data>) const = 0;
	};
}