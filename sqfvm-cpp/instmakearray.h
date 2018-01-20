#ifndef _INSTMAKEARRAY
#define _INSTMAKEARRAY 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instmakearray requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instmakearray requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instmakearray requires "instruction.h" header
#endif // !_INSTRUCTION

namespace sqf
{
	namespace inst
	{
		class makearray : public instruction
		{
		private:
			size_t msize;
		public:
			makearray(size_t size) { msize = size; }
			virtual void execute(const virtualmachine* vm) const;
		};
		typedef std::shared_ptr<makearray> makearray_s;
		typedef std::weak_ptr<makearray> makearray_w;
		typedef std::unique_ptr<makearray> makearray_u;
	}
}

#endif // !_INSTMAKEARRAY
