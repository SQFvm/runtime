#ifndef _INSTASSIGNTO
#define _INSTASSIGNTO 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instassignto requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instassignto requires <stack> header
#endif // !_STACK
#if !defined(_STRING) & !defined(_STRING_)
#error instassignto requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error instassignto requires <instruction> header
#endif // !_INSTRUCTION

namespace sqf
{
	namespace inst
	{
		class assignto : public instruction
		{
		private:
			std::wstring mvarname;
		public:
			assignto(std::wstring varname) { mvarname = varname; }
			virtual void execute(const virtualmachine*) const;
			virtual insttype thistype(void) const { return insttype::assignto; }
		};
		typedef std::shared_ptr<assignto> assignto_s;
		typedef std::weak_ptr<assignto> assignto_w;
		typedef std::unique_ptr<assignto> assignto_u;
	}
}

#endif // !_INSTASSIGNTO
