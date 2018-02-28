#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "sqffull.h"

PCMD SCALAR_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"SCALAR", 't', 0, 0, NULL, NULL, NULL);
	}
	return cmd;
}


extern inline VALUE addScalarPointer(const PVALUE left, const PVALUE right);
extern inline VALUE addScalar(const VALUE left, const VALUE right);
extern inline VALUE substractScalarPointer(const PVALUE left, const PVALUE right);
extern inline VALUE substractScalar(const VALUE left, const VALUE right);
extern inline VALUE multiplyScalarPointer(const PVALUE left, const PVALUE right);
extern inline VALUE multiplyScalar(const VALUE left, const VALUE right);
extern inline VALUE divideScalarPointer(const PVALUE left, const PVALUE right);
extern inline VALUE divideScalar(const VALUE left, const VALUE right);