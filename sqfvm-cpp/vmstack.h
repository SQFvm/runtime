#ifndef _VMSTACK
#define _VMSTACK 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error vmstack requires <memory> header
#endif // !_MEMORY
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error vmstack requires <vector> header
#endif // !_VECTOR
#if !defined(_WSTRING_OP_H_)
#error vmstack requires "wstring_op.h" header
#endif // !_VECTOR

namespace sqf
{
	class vmstack
	{
	private:
		std::vector<callstack_s> mstacks;
		std::vector<value_s> mvalstack;
	public:
		inline void pushinst(std::shared_ptr<instruction> inst) { mstacks.back()->pushinst(inst); }
		inline std::shared_ptr<instruction> popinst(void) { if (mstacks.empty()) return instruction_s(); return mstacks.back()->popinst(); }
		inline std::shared_ptr<instruction> peekinst(void) { if (mstacks.empty()) return instruction_s(); return mstacks.back()->peekinst(); }
		inline void pushcallstack(callstack_s cs) { mstacks.push_back(cs); }
		inline void dropcallstack(std::wstring name, bool include = true)
		{
			int i;
			for (i = mstacks.size() - 1; i >= 0; i--)
			{
				auto stack = mstacks[i];
				if (wstr_cmpi(stack->getscopename().c_str(), -1, name.c_str(), -1) == 0)
				{
					i = mstacks.size() - i;
					if (include)
					{
						i++;
					}
					for (; i > 0; i--)
					{
						mstacks.pop_back();
					}
					break;
				}
			}
		}

		inline std::vector<callstack_s>::reverse_iterator stacks_begin(void) { return mstacks.rbegin(); }
		inline std::vector<callstack_s>::reverse_iterator stacks_end(void) { return mstacks.rend(); }
		inline callstack_s stacks_top(void) { return mstacks.back(); }

		inline void pushval(value_s val) { mvalstack.push_back(val); }
		inline value_s popval(bool &success) { if (mvalstack.empty()) { success = false; return value_s(); } success = true; auto val = mvalstack.back(); mvalstack.pop_back(); return val; }
		inline value_s peekval(void) { if (mvalstack.empty()) return value_s(); return mvalstack.back(); }
		inline void dropvals(void) { mvalstack.clear(); }
	};
	typedef std::shared_ptr<vmstack> vmstack_s;
	typedef std::weak_ptr<vmstack> vmstack_w;
	typedef std::unique_ptr<vmstack> vmstack_u;
}

#endif // !_VMSTACK
