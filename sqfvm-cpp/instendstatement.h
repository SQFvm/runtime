#ifndef _INSTENDSTATEMENT
#define _INSTENDSTATEMENT 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instendstatement requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instendstatement requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instendstatement requires <instruction> header
#endif // !_INSTRUCTION

namespace sqf
{
	namespace inst
	{
		class endstatement : public instruction
		{
		public:
			virtual void execute(const virtualmachine* vm) const { vm->stack()->dropvals(); }
			virtual insttype thistype(void) const { return insttype::endstatement; }
		};
		typedef std::shared_ptr<endstatement> endstatement_s;
		typedef std::weak_ptr<endstatement> endstatement_w;
		typedef std::unique_ptr<endstatement> endstatement_u;
	}
}

#endif // !_INSTENDSTATEMENT
