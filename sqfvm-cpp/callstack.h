#ifndef _CALLSTACK
#define _CALLSTACK 1


#if !defined(_MEMORY) & !defined(_MEMORY_)
#error callstack requires <memory> header
#endif // !_MEMORY
#if !defined(_QUEUE) & !defined(_QUEUE_)
#error callstack requires <queue> header
#endif // !_QUEUE
#if !defined(_STRING) & !defined(_STRING_)
#error callstack requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error callstack requires "instruction.h" header
#endif // !_INSTRUCTION
#if !defined(_NAMESPACE)
#error callstack requires "namespace.h" header
#endif // !_NAMESPACE

namespace sqf
{
	class callstack : public varscope
	{
	private:
		std::queue<instruction_s> mstack;
		sqfnamespace& mwith;
	public:
		callstack();
		callstack(sqfnamespace&);
		inline void pushinst(instruction_s value) { mstack.push(value); }
		inline instruction_s popinst(void) { if (mstack.empty()) return instruction_s(); auto ret = mstack.front(); mstack.pop(); return ret; }
		inline instruction_s peekinst(void) { if (mstack.empty()) return instruction_s(); auto front = mstack.front(); return front; }
		inline sqfnamespace& getnamespace(void) const { return mwith; }
		inline size_t inststacksize(void) const { return mstack.size(); }
		inline void setnamespace(sqfnamespace& ns) { mwith = ns; }
	};

	typedef std::shared_ptr<callstack> callstack_s;
	typedef std::weak_ptr<callstack> callstack_w;
	typedef std::unique_ptr<callstack> callstack_u;
}

#endif // !_CALLSTACK
