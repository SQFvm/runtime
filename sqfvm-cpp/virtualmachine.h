#ifndef _SQFVM
#define _SQFVM 1

#if !defined(_TYPEINFO) & !defined(_TYPEINFO_)
#error sqfvm requires <typeinfo> header
#endif // !_TYPEINFO
#if !defined(_STRING) & !defined(_STRING_)
#error sqfvm requires <string> header
#endif // !_STRING
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error sqfvm requires <vector> header
#endif // !_STRING

#ifndef _VALUE
#error sqfvm requires "value" header
#endif // !_VALUE

namespace sqf
{
	class virtualmachine
	{
	public:
		virtualmachine(int stacksize, int worksize);
	};
	typedef std::shared_ptr<virtualmachine> virtualmachine_s;
	typedef std::weak_ptr<virtualmachine> virtualmachine_w;
	typedef std::unique_ptr<virtualmachine> virtualmachine_u;
}
#endif // !_SQFVM