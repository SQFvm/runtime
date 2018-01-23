#ifndef _INSTCALLNULAR
#define _INSTCALLNULAR 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instcallnular requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instcallnular requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instcallnular requires "instruction" header
#endif // !_INSTRUCTION
#if !defined(_CMD)
#error instcallnular requires "cmd.h" header
#endif // !_CMD

namespace sqf
{
	namespace inst
	{
		class callnular : public instruction
		{
		private:
			cmd_s mcmd;
		public:
			callnular(cmd_s cmd) { mcmd = cmd; }
			virtual void execute(const virtualmachine*) const;
		};
		typedef std::shared_ptr<callnular> callnular_s;
		typedef std::weak_ptr<callnular> callnular_w;
		typedef std::unique_ptr<callnular> callnular_u;
	}
}

#endif // !_INSTCALLNULAR
