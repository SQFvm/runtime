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
		return 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665;
	}
	static constexpr double rand_lim(int rnd, double limit)
	{
		return (((double)rnd) / RAND_MAX) * limit;
	}
};

#endif // !_COMPILETIME
