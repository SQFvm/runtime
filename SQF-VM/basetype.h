#ifndef _BASETYPE_H_
#define _BASETYPE_H_


typedef union BASE
{
	float f;
	int i;
	char c;
	void* ptr;
	const void* cptr;
	long l;
	//long float lf;
	//long long ll;
} BASE;

inline BASE base_float(float f)
{
	BASE b;
	b.f = f;
	return b;
}
inline BASE base_int(int i)
{
	BASE b;
	b.i = i;
	return b;
}
inline BASE base_char(char c)
{
	BASE b;
	b.c = c;
	return b;
}
inline BASE base_voidptr(void* ptr)
{
	BASE b;
	b.ptr = ptr;
	return b;
}
inline BASE base_long(long l)
{
	BASE b;
	b.l = l;
	return b;
}
//inline BASE base_longfloat(long float lf)
//{
//	BASE b;
//	b.lf = lf;
//	return b;
//}
//inline BASE base_longlong(long long ll)
//{
//	BASE b;
//	b.ll = ll;
//	return b;
//}

#endif