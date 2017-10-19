#include <ctype.h>
#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>
#include "wstring_op.h"


unsigned int wstr_sw(const wchar_t* lString, const wchar_t* rString)
{
	unsigned int i;
	wchar_t lc, rc;
	for (i = 0; lString[i] != '\0'; i++)
	{
		lc = lString[i];
		rc = rString[i];
		if (rc == '\0')
			return 0;
		if (lc != rc)
			return i + 1;
	}
	return i;
}
unsigned int wstr_swi(const wchar_t* lString, const wchar_t* rString)
{
	unsigned int i;
	wchar_t lc, rc;
	for (i = 0; lString[i] != '\0'; i++)
	{
		lc = towlower(lString[i]);
		rc = towlower(rString[i]);
		if (rc == '\0')
			return 0;
		if (lc != rc)
			return i + 1;
	}
	return i;
}

unsigned int wstr_ew(const wchar_t* lString, const wchar_t* rString)
{
	unsigned int i;
	unsigned int rlen = wcslen(rString);
	wchar_t lc, rc;
	for (i = 0; lString[i] != '\0'; i++);
	if (i < rlen)
		return 0;
	i -= rlen;
	for (; lString[i] != '\0'; i++)
	{
		lc = lString[i];
		rc = rString[i];
		if (rc == '\0')
			return 1;
		if (lc != rc)
			return 0;
	}
	return 0;
}
unsigned int wstr_ewi(const wchar_t* lString, const wchar_t* rString)
{
	unsigned int i;
	unsigned int rlen = wcslen(rString);
	wchar_t lc, rc;
	for (i = 0; lString[i] != '\0'; i++);
	if (i < rlen)
		return 0;
	i -= rlen;
	for (; lString[i] != '\0'; i++)
	{
		lc = towlower(lString[i]);
		rc = towlower(rString[i]);
		if (rc == '\0')
			return 1;
		if (lc != rc)
			return 0;
	}
	return 0;
}

const wchar_t* wstr_strwrd(const wchar_t* lString, const wchar_t* rString, const wchar_t* letters)
{
	int i, j;
	wchar_t lc, rc;
	bool flag = false;
	bool isSeparated = true;

	if (letters == NULL)
		letters = L" ,-_\t.?!+:;<>#";
	for (i = 0, j = 0;; i++, j++)
	{
		lc = lString[i];
		rc = rString[j];
		if (flag)
		{
			if (rc == '\0')
			{
				if (wchr_is(lc, letters) || lc == '\0')
					return lString + i - j;
				flag = false;
				isSeparated = false;
			}
			if (lc != rc)
			{
				flag = false;
				isSeparated = wchr_is(lc, letters);
			}
		}
		else if (!isSeparated)
		{
			if (wchr_is(lc, letters))
				isSeparated = true;
			j = -1;
		}
		else if (lc == rc)
		{
			flag = true;
		}
		else
		{
			isSeparated = wchr_is(lc, letters);
			j = -1;
		}
		if (lc == '\0')
			return NULL;
	}
}

const wchar_t* wstr_strwrdi(const wchar_t* lString, const wchar_t* rString, const wchar_t* letters)
{
	int i, j;
	wchar_t lc, rc;
	bool flag = false;
	bool isSeparated = true;

	if (letters == NULL)
		letters = L" ,-_\t";
	for (i = 0, j = 0;; i++, j++)
	{
		lc = towlower(lString[i]);
		rc = towlower(rString[j]);
		if (flag)
		{
			if (rc == '\0')
			{
				if (wchr_is(lc, letters) || lc == '\0')
					return lString + i - j;
				flag = false;
				isSeparated = false;
			}
			if (lc != rc)
			{
				flag = false;
				isSeparated = wchr_is(lc, letters);
			}
		}
		else if (!isSeparated)
		{
			if (wchr_is(lc, letters))
				isSeparated = true;
			j = -1;
		}
		else if (lc == rc)
		{
			flag = true;
		}
		else
		{
			isSeparated = wchr_is(lc, letters);
			j = -1;
		}
		if (lc == '\0')
			return NULL;
	}
}

int wchr_is(const wchar_t c, const wchar_t* isArr)
{
	unsigned int i;
	for (i = 0; isArr[i] != '\0'; i++)
	{
		if (isArr[i] == c)
			return 1;
	}
	return 0;
}


unsigned int wstr_repchr(wchar_t* str, wchar_t toFind, wchar_t toReplace, int length)
{
	int i, j = 0;
	if (length == -1)
	{
		length = wcslen(str);
	}
	for (i = 0; i < length; i++)
	{
		if (str[i] == toFind)
		{
			str[i] = toReplace;
			j++;
		}
	}
	return j;
}


unsigned int wstr_cmp(const wchar_t* lString, int lLen, const wchar_t* rString, int rLen)
{
	int i;
	if (lLen != -1 && rLen != -1 && lLen != rLen)
		return -1;
	i = 0;
	while ((lLen != -1 && rLen != -1 && i < lLen) || lString[i] != '\0' || rString[i] != '\0')
	{
		if (lString[i] != rString[i])
		{
			return -1;
		}
	}

	return 0;
}
unsigned int wstr_cmpi(const wchar_t* lString, int lLen, const wchar_t* rString, int rLen)
{
	int i;
	if (lLen != -1 && rLen != -1 && lLen != rLen)
		return -1;
	i = 0;
	while ((lLen != -1 && rLen != -1 && i < lLen) || lString[i] != '\0' || rString[i] != '\0')
	{
		if (towlower(lString[i]) != towlower(rString[i]))
		{
			return -1;
		}
		i++;
	}

	return 0;
}
