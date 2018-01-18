#ifndef _INSTASSIGNTOLOCAL
#define _INSTASSIGNTOLOCAL 1

#if !defined(_MEMORY) & !defined(_MEMORY_)
#error instassigntolocal requires <memory> header
#endif // !_MEMORY
#if !defined(_STACK) & !defined(_STACK_)
#error instassigntolocal requires <stack> header
#endif // !_STACK
#if !defined(_STRING) & !defined(_STRING_)
#error instassigntolocal requires <string> header
#endif // !_STRING
#if !defined(_INSTRUCTION)
#error instassigntolocal requires <instruction> header
#endif // !_INSTRUCTION

namespace sqf
{
	namespace inst
	{
		class assigntolocal : public instruction
		{
		private:
			std::wstring mvarname;
		public:
			assigntolocal(std::wstring varname) { mvarname = varname; }
			virtual void execute(const virtualmachine*) const;
		};
		typedef std::shared_ptr<assigntolocal> assigntolocal_s;
		typedef std::weak_ptr<assigntolocal> assigntolocal_w;
		typedef std::unique_ptr<assigntolocal> assigntolocal_u;
	}
}

#endif // !_INSTASSIGNTOLOCAL
