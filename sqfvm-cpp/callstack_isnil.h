#ifndef _CALLSTACK_ISNIL
#define _CALLSTACK_ISNIL 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error callstack_isnil requires <memory> header
#endif // !_MEMORY
#if !defined(_QUEUE) & !defined(_QUEUE_)
#error callstack_isnil requires <queue> header
#endif // !_QUEUE
#if !defined(_STRING) & !defined(_STRING_)
#error callstack_isnil requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error callstack_isnil requires "instruction.h" header
#endif // !_INSTRUCTION
#if !defined(_NAMESPACE)
#error callstack_isnil requires "namespace.h" header
#endif // !_NAMESPACE
#if !defined(_CALLSTACK)
#error callstack_isnil requires "callstack.h" header
#endif // !_CALLSTACK

namespace sqf
{
	class callstack_isnil : public callstack
	{
	public:
		callstack_isnil(const sqf::virtualmachine * vm, std::shared_ptr<codedata> exec);
		virtual instruction_s popinst(const sqf::virtualmachine* vm);
	};
}

#endif // !_CALLSTACK_ISNIL
