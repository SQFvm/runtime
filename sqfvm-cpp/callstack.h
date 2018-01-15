#ifndef _CALLSTACK
#define _CALLSTACK 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error callstack requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error callstack requires <stack> header
#endif // !_STACK
#if !defined(_STRING) & !defined(_STRING_)
#error callstack requires <string> header
#endif // !_STRING

namespace sqf
{
	class callstack : public varscope
	{
	private:
		std::stack<instruction_s> mstack;
	public:
		inline void pushinst(instruction_s value) { mstack.push(value); }
		inline instruction_s popinst(void) { auto ret = mstack.empty() ? instruction_s() : mstack.top(); mstack.pop(); return ret; }
		inline instruction_s peekinst(void) { if (mstack.empty()) return instruction_s(); auto top = mstack.top(); return top; }
	};

	typedef std::shared_ptr<callstack> callstack_s;
	typedef std::weak_ptr<callstack> callstack_w;
	typedef std::unique_ptr<callstack> callstack_u;
}

#endif // !_CALLSTACK
