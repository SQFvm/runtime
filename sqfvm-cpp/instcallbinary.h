#ifndef _INSTCALLBINARY
#define _INSTCALLBINARY 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instcallbinary requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instcallbinary requires <stack> header
#endif // !_STACK
#if !defined(_INSTRUCTION)
#error instcallbinary requires "instruction" header
#endif // !_INSTRUCTION
#if !defined(_CMD)
#error instcallbinary requires "cmd.h" header
#endif // !_CMD

namespace sqf
{
	namespace inst
	{
		class callbinary : public instruction
		{
		private:
			std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> mcmds;
		public:
			callbinary(std::shared_ptr<std::vector<std::shared_ptr<binarycmd>>> cmds) { mcmds = cmds; }
			virtual void execute(const virtualmachine*) const;
		};
		typedef std::shared_ptr<callbinary> callbinary_s;
		typedef std::weak_ptr<callbinary> callbinary_w;
		typedef std::unique_ptr<callbinary> callbinary_u;
	}
}

#endif // !_INSTCALLBINARY
