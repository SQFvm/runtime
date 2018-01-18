#ifndef _INSTPUSH
#define _INSTPUSH 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instpush requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instpush requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instpush requires <instruction> header
#endif // !_INSTRUCTION
#if !defined(_VALUE)
#error instpush requires <instruction> header
#endif // !_VALUE

namespace sqf
{
	namespace inst
	{
		class push : public instruction
		{
		private:
			value_s mvalue;
		public:
			push(value_s val) { mvalue = val; }
			virtual void execute(const virtualmachine* vm) const { vm->stack()->pushval(mvalue); }
		};
		typedef std::shared_ptr<push> push_s;
		typedef std::weak_ptr<push> push_w;
		typedef std::unique_ptr<push> push_u;
	}
}

#endif // !_INSTPUSH
