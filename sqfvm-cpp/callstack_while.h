#ifndef _CALLSTACK_WHILE
#define _CALLSTACK_WHILE 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error callstack_while requires <memory> header
#endif // !_MEMORY
#if !defined(_QUEUE) & !defined(_QUEUE_)
#error callstack_while requires <queue> header
#endif // !_QUEUE
#if !defined(_STRING) & !defined(_STRING_)
#error callstack_while requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error callstack_while requires "instruction.h" header
#endif // !_INSTRUCTION
#if !defined(_NAMESPACE)
#error callstack_while requires "namespace.h" header
#endif // !_NAMESPACE
#if !defined(_CALLSTACK)
#error callstack_while requires "callstack.h" header
#endif // !_CALLSTACK

namespace sqf
{
	class callstack_while : public callstack
	{
	private:
		std::shared_ptr<codedata> mwhilecond;
		std::shared_ptr<codedata> mexec;
		bool mdoexec;
	public:
		callstack_while(std::shared_ptr<codedata> whilecond, std::shared_ptr<codedata> exec) : mwhilecond(whilecond), mexec(exec), mdoexec(false) {}
		virtual instruction_s popinst(const sqf::virtualmachine* vm);
	};
}

#endif // !_CALLSTACK_WHILE
