#ifndef _COMPILETIME
#define _COMPILETIME 1

#if !defined(_MATH) && !defined(_MATH_)
#include <cmath>
#endif
#if !defined(_RANDOM) && !defined(_RANDOM_)
#include <random>
#endif

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
	static constexpr double pi()
	{
		return std::atan(1) * 4;
	}
	static constexpr double rand(double limit)
	{
		return (((double)std::rand()) / RAND_MAX) * limit;
	}
};

#endif // !_COMPILETIME
