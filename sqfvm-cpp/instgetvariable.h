#ifndef _INSTGETVARIABLE
#define _INSTGETVARIABLE 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instgetvariable requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instgetvariable requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instgetvariable requires <instruction> header
#endif // !_INSTRUCTION
#if !defined(_STRING) & !defined(_STRING_)
#error instgetvariable requires <string> header
#endif // !_STRING

namespace sqf
{
	namespace inst
	{
		class getvariable : public instruction
		{
		private:
			std::wstring mvarname;
		public:
			getvariable(std::wstring varname) { mvarname = varname; }
			virtual void execute(const virtualmachine*) const;
			virtual insttype thistype(void) const { return insttype::getvariable; }
		};
		typedef std::shared_ptr<getvariable> getvariable_s;
		typedef std::weak_ptr<getvariable> getvariable_w;
		typedef std::unique_ptr<getvariable> getvariable_u;
	}
}

#endif // !_INSTGETVARIABLE
