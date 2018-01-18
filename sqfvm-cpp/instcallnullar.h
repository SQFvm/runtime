#ifndef _INSTCALLNULLAR
#define _INSTCALLNULLAR 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instcallnullar requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instcallnullar requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instcallnullar requires "instruction" header
#endif // !_INSTRUCTION
#if !defined(_CMD)
#error instcallnullar requires "cmd.h" header
#endif // !_CMD

namespace sqf
{
	namespace inst
	{
		class callnullar : public instruction
		{
		private:
			cmd_s mcmd;
		public:
			callnullar(cmd_s cmd) { mcmd = cmd; }
			virtual void execute(const virtualmachine*) const;
		};
		typedef std::shared_ptr<callnullar> callnullar_s;
		typedef std::weak_ptr<callnullar> callnullar_w;
		typedef std::unique_ptr<callnullar> callnullar_u;
	}
}

#endif // !_INSTCALLNULLAR
