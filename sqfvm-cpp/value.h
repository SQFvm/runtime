#ifndef _VALUE
#define _VALUE ::sqfvm::value

#if !defined(_TYPEINFO) & !defined(_TYPEINFO_)
#error value requires <typeinfo> header
#endif // !_TYPEINFO
#if !defined(_STRING) & !defined(_STRING_)
#error value requires <string> header
#endif // !_STRING

namespace sqfvm
{
	class value
	{
	public:
		virtual value operator +(const value l) const;
		virtual value operator -(const value l) const;
		virtual value operator *(const value l) const;
		virtual value operator /(const value l) const;
		virtual value operator ==(const value l) const;
		virtual value create_copy(void) const;
		virtual std::string get_typename(void) const;
	};
}
#endif // !_GAMEVALUE