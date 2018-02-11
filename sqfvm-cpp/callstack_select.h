#ifndef _CALLSTACK_SELECT
#define _CALLSTACK_SELECT 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error callstack_select requires <memory> header
#endif // !_MEMORY
#if !defined(_QUEUE) & !defined(_QUEUE_)
#error callstack_select requires <queue> header
#endif // !_QUEUE
#if !defined(_STRING) & !defined(_STRING_)
#error callstack_select requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error callstack_select requires "instruction.h" header
#endif // !_INSTRUCTION
#if !defined(_NAMESPACE)
#error callstack_select requires "namespace.h" header
#endif // !_NAMESPACE
#if !defined(_CALLSTACK)
#error callstack_select requires "callstack.h" header
#endif // !_CALLSTACK

namespace sqf
{
	class callstack_select : public callstack
	{
	private:
		size_t mcurindex;
		bool mend;
		std::vector<value_s> marr;
		std::vector<value_s> moutarr;
		std::shared_ptr<codedata> mcond;
	public:
		callstack_select(std::vector<value_s> arr, std::shared_ptr<codedata> cond) : marr(arr), mcond(cond), mcurindex(0), mend(arr.size() == 0) {}
		virtual instruction_s popinst(const sqf::virtualmachine* vm);
	};
}

#endif // !_CALLSTACK_SELECT
