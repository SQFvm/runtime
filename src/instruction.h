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
		std::string mfile;
		std::string msegment;
	public:
		size_t line() { return mline; }
		size_t col() { return mcol; }
		std::string file() { return mfile; }
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
		void setdbginf(size_t line, size_t col, std::string file, std::string segment);
		std::string dbginf(std::string tag) const;
		virtual insttype thistype() const = 0;
		virtual std::string to_string() const = 0;
	};
}