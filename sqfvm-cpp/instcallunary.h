#ifndef _INSTCALLUNARY
#define _INSTCALLUNARY 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instcallunary requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instcallunary requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instcallunary requires "instruction" header
#endif // !_INSTRUCTION
#if !defined(_CMD)
#error instcallunary requires "cmd.h" header
#endif // !_CMD

namespace sqf
{
	namespace inst
	{
		class callunary : public instruction
		{
		private:
			cmd_s mcmd;
		public:
			callunary(cmd_s cmd) { mcmd = cmd; }
			virtual void execute(const virtualmachine*, std::shared_ptr<vmstack>) const;
		};
		typedef std::shared_ptr<callunary> callunary_s;
		typedef std::weak_ptr<callunary> callunary_w;
		typedef std::unique_ptr<callunary> callunary_u;
	}
}

#endif // !_INSTCALLUNARY
