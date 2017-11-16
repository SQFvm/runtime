#ifndef _SQFVM
#define _SQFVM ::sqfvm::sqfvm

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

namespace sqfvm
{
	class sqfvm
	{
	public:
		sqfvm(int stacksize, int worksize);
	};
}
#endif // !_SQFVM