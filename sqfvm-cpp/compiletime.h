#ifndef _COMPILETIME
#define _COMPILETIME 1

class compiletime
{
private:
	compiletime();
public:
	static unsigned int constexpr strlen(const char* str)
	{
		return str[0] ? 1 + strlen(str + 1) : 0;
	}
	static unsigned int constexpr strlen(const wchar_t* str)
	{
		return str[0] ? 1 + strlen(str + 1) : 0;
	}
};

#endif // !_COMPILETIME
