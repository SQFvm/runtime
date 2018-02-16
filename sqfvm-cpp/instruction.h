#pragma once
#include <string>
#include "instruction.h"

namespace sqf
{
	class vmstack;
	class virtualmachine;
	class instruction
	{
	private:
		size_t mline;
		size_t mcol;
		std::wstring mfile;
		std::wstring msegment;
	public:
		enum insttype
		{
			push,
			assignto,
			assigntolocal,
			callnular,
			callunary,
			callbinary,
			endstatement,
			makearray,
			getvariable
		};
		virtual void execute(virtualmachine*) const = 0;
		void setdbginf(size_t line, size_t col, std::wstring file, std::wstring segment);
		std::wstring dbginf(std::wstring tag) const;
		virtual insttype thistype(void) const = 0;
	};
}